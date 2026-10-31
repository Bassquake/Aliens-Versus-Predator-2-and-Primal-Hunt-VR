// ----------------------------------------------------------------------- //
//
// MODULE  : PrimalHuntClient.cpp
//
// PURPOSE : Lets the 1.0.9.6 client code run on Primal Hunt's engine
//
// Primal Hunt's lithtech.exe added virtuals to ILTClient and ILTCommon, moving
// the ones after them. The prebuilt libraries (ltguimgr, LithFontMgr) can't be
// rebuilt and keep the old slots, so instead of changing the SDK the client
// gets a copy of each interface whose vtable has the old layout. Each entry is
// a thunk that swaps in the real object and jumps to the moved slot. ILTClient's
// function pointer members didn't move, so the copy can call them directly.
//
// ----------------------------------------------------------------------- //

#include "stdafx.h"
#include "PrimalHuntClient.h"

#ifdef AVP2_PRIMAL_HUNT

#include <windows.h>
#include <stdlib.h>
#include <string.h>

// ILTClient: two new virtuals before FormatString, one before StartQuery
#define CLIENT_SLOTS				110
#define CLIENT_FORMATSTRING_SLOT	44
#define CLIENT_STARTQUERY_SLOT		106

// ILTCommon: GetPolyTextureName is new, before GetPolyInfo
#define COMMON_SLOTS				21
#define COMMON_GETPOLYINFO_SLOT		19

// Variadic members are __cdecl, so 'this' is on the stack rather than in ecx
#define CLIENT_WRITEFORMATTED_SLOT	13
#define CLIENT_CPRINT_SLOT			39

#define THUNK_SIZE					16

// Only here to reach the protected interface pointer of the copy
class CClientMembers : public ILTClient
{
public:
	ILTCommon*& CommonPtr() { return m_pCommonLT; }
};

static int NewClientSlot(int nSlot)
{
	if(nSlot < CLIENT_FORMATSTRING_SLOT)
		return nSlot;

	return nSlot < CLIENT_STARTQUERY_SLOT ? nSlot + 2 : nSlot + 3;
}

static int NewCommonSlot(int nSlot)
{
	return nSlot < COMMON_GETPOLYINFO_SLOT ? nSlot : nSlot + 1;
}

static BYTE* WriteThunk(BYTE *pCode, void *pThis, void **ppTarget, LTBOOL bThisOnStack)
{
	if(bThisOnStack)
	{
		// mov dword ptr [esp+4], pThis
		*pCode++ = 0xC7;
		*pCode++ = 0x44;
		*pCode++ = 0x24;
		*pCode++ = 0x04;
	}
	else
	{
		// mov ecx, pThis
		*pCode++ = 0xB9;
	}

	memcpy(pCode, &pThis, sizeof(pThis));
	pCode += sizeof(pThis);

	// jmp dword ptr [ppTarget]
	*pCode++ = 0xFF;
	*pCode++ = 0x25;
	memcpy(pCode, &ppTarget, sizeof(ppTarget));
	pCode += sizeof(ppTarget);

	return pCode;
}

// A copy of pObject whose vtable (at pTable, nSlots long) is thunks at pCode
template<class T>
static T* CopyWithVTable(T *pObject, void **pTable, BYTE *pCode, int nSlots, int (*pNewSlot)(int), LTBOOL bClient)
{
	T *pCopy = (T*)malloc(sizeof(T));
	if(!pCopy)
		return LTNULL;

	memcpy(pCopy, pObject, sizeof(T));

	void **pRealTable = *(void***)pObject;

	for(int i = 0; i < nSlots; i++)
	{
		LTBOOL bThisOnStack = bClient &&
			(i == CLIENT_WRITEFORMATTED_SLOT || i == CLIENT_CPRINT_SLOT || i == CLIENT_FORMATSTRING_SLOT);

		pTable[i] = pCode;
		WriteThunk(pCode, pObject, &pRealTable[pNewSlot(i)], bThisOnStack);
		pCode += THUNK_SIZE;
	}

	*(void***)pCopy = pTable;
	return pCopy;
}

ILTClient* PrimalHunt_WrapClient(ILTClient *pClientDE)
{
	int nCodeSize = (CLIENT_SLOTS + COMMON_SLOTS) * THUNK_SIZE;
	BYTE *pCode = (BYTE*)VirtualAlloc(LTNULL, nCodeSize, MEM_COMMIT | MEM_RESERVE, PAGE_EXECUTE_READWRITE);
	void **pClientTable = (void**)malloc(CLIENT_SLOTS * sizeof(void*));
	void **pCommonTable = (void**)malloc(COMMON_SLOTS * sizeof(void*));

	if(!pCode || !pClientTable || !pCommonTable)
		return pClientDE;

	BYTE *pCommonCode = pCode + CLIENT_SLOTS * THUNK_SIZE;

	ILTClient *pClient = CopyWithVTable(pClientDE, pClientTable, pCode, CLIENT_SLOTS, NewClientSlot, LTTRUE);
	ILTCommon *pCommon = CopyWithVTable(pClientDE->Common(), pCommonTable, pCommonCode, COMMON_SLOTS, NewCommonSlot, LTFALSE);

	if(!pClient || !pCommon)
		return pClientDE;

	((CClientMembers*)pClient)->CommonPtr() = pCommon;

	FlushInstructionCache(GetCurrentProcess(), pCode, nCodeSize);
	return pClient;
}

#endif // AVP2_PRIMAL_HUNT
