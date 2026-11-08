// Implicit OpenXR API layers that can't load in this 32-bit process.
//
// Overlays, toolkits and hand-tracking packages register "implicit" layers that the OpenXR loader loads
// into every app. A layer registered for 32-bit apps (HKLM\SOFTWARE\WOW6432Node\Khronos\OpenXR\1\ApiLayers\
// Implicit, or HKCU, which 32- and 64-bit apps share) whose DLL is missing, or is 64-bit only, makes the
// loader fail xrCreateInstance with XR_ERROR_FILE_ACCESS_ERROR ("Failed loading layer information")
// instead of skipping it. Every implicit layer manifest names a disable_environment variable, so set that
// for each such layer before creating the instance. Layers that would load are left alone.
// Include after <windows.h>; logf receives one finished line per disabled layer.
#pragma once
#include <string>

namespace xrlayers {

static std::string JsonString(const std::string& json, const char* key)
{
	std::string quoted = std::string("\"") + key + "\"";
	size_t p = json.find(quoted);
	if (p == std::string::npos)
		return "";
	p = json.find(':', p + quoted.size());
	if (p == std::string::npos)
		return "";
	p = json.find('"', p + 1);
	if (p == std::string::npos)
		return "";
	std::string out;
	for (++p; p < json.size() && json[p] != '"'; ++p)
	{
		if (json[p] == '\\' && p + 1 < json.size())
			++p;  // \\ and \/ are all a path or variable name can contain
		out += json[p];
	}
	return out;
}

static std::wstring Utf8ToWide(const std::string& s)
{
	int n = MultiByteToWideChar(CP_UTF8, 0, s.c_str(), (int)s.size(), nullptr, 0);
	std::wstring w(n, L'\0');
	if (n)
		MultiByteToWideChar(CP_UTF8, 0, s.c_str(), (int)s.size(), &w[0], n);
	return w;
}

static bool ReadFileBytes(const std::wstring& path, std::string& data, DWORD maxBytes)
{
	HANDLE f = CreateFileW(path.c_str(), GENERIC_READ, FILE_SHARE_READ | FILE_SHARE_WRITE, nullptr, OPEN_EXISTING, 0, nullptr);
	if (f == INVALID_HANDLE_VALUE)
		return false;
	data.resize(maxBytes);
	DWORD got = 0;
	BOOL ok = ReadFile(f, &data[0], maxBytes, &got, nullptr);
	CloseHandle(f);
	data.resize(ok ? got : 0);
	return ok != FALSE;
}

// Why the DLL can't be loaded by this process, or nullptr if it looks loadable.
static const char* DllProblem(const std::wstring& path)
{
	std::string pe;
	if (!ReadFileBytes(path, pe, 4096))
		return "its DLL is missing";
	if (pe.size() < sizeof(IMAGE_DOS_HEADER))
		return "its DLL is not a valid DLL";
	const IMAGE_DOS_HEADER* dos = (const IMAGE_DOS_HEADER*)pe.data();
	if (dos->e_magic != IMAGE_DOS_SIGNATURE || dos->e_lfanew < 0 ||
		(size_t)dos->e_lfanew + sizeof(DWORD) + sizeof(IMAGE_FILE_HEADER) > pe.size())
		return "its DLL is not a valid DLL";
	const char* nt = pe.data() + dos->e_lfanew;
	if (*(const DWORD*)nt != IMAGE_NT_SIGNATURE)
		return "its DLL is not a valid DLL";
	const IMAGE_FILE_HEADER* fh = (const IMAGE_FILE_HEADER*)(nt + sizeof(DWORD));
#ifdef _WIN64
	return fh->Machine == IMAGE_FILE_MACHINE_AMD64 ? nullptr : "its DLL is not 64-bit";
#else
	return fh->Machine == IMAGE_FILE_MACHINE_I386 ? nullptr : "its DLL is 64-bit only";
#endif
}

static void CheckManifest(const std::wstring& manifest, void (*logf)(const char* line))
{
	std::string json;
	if (!ReadFileBytes(manifest, json, 1 << 16))
		return;  // the loader skips unreadable manifests itself
	std::string name = JsonString(json, "name");
	std::string disableVar = JsonString(json, "disable_environment");
	std::wstring lib = Utf8ToWide(JsonString(json, "library_path"));
	if (lib.empty() || disableVar.empty())
		return;
	if (lib.find_first_of(L"\\/") == std::wstring::npos)
		return;  // a bare file name is found on the DLL search path; can't check it here
	if (!(lib.size() > 1 && (lib[1] == L':' || (lib[0] == L'\\' && lib[1] == L'\\'))))
	{
		size_t slash = manifest.find_last_of(L"\\/");
		lib = (slash == std::wstring::npos ? L"" : manifest.substr(0, slash + 1)) + lib;  // relative to the manifest
	}
	const char* problem = DllProblem(lib);
	if (!problem)
		return;
	SetEnvironmentVariableA(disableVar.c_str(), "1");
	char line[1024];
	sprintf_s(line, "Disabled OpenXR API layer %s (%ls): %s (%ls)", name.c_str(), manifest.c_str(), problem, lib.c_str());
	logf(line);
}

static void CheckRegistry(HKEY root, void (*logf)(const char* line))
{
	HKEY key;
	if (RegOpenKeyExW(root, L"SOFTWARE\\Khronos\\OpenXR\\1\\ApiLayers\\Implicit", 0, KEY_READ, &key) != ERROR_SUCCESS)
		return;
	for (DWORD i = 0;; ++i)
	{
		wchar_t manifest[1024];
		DWORD nameLen = 1024, type = 0, value = 1, valueSize = sizeof(value);
		LONG r = RegEnumValueW(key, i, manifest, &nameLen, nullptr, &type, (BYTE*)&value, &valueSize);
		if (r == ERROR_NO_MORE_ITEMS)
			break;
		if (r == ERROR_SUCCESS && type == REG_DWORD && value == 0)  // 0 = enabled
			CheckManifest(manifest, logf);
	}
	RegCloseKey(key);
}

// HKLM here is the registry view of this process's bitness (WOW6432Node for 32-bit), as the loader uses.
static void DisableUnloadable(void (*logf)(const char* line))
{
	CheckRegistry(HKEY_LOCAL_MACHINE, logf);
	CheckRegistry(HKEY_CURRENT_USER, logf);
}

}  // namespace xrlayers
