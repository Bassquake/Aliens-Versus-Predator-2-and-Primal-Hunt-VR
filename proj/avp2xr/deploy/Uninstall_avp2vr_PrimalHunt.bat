@echo off
rem Removes everything Install_avp2vr_PrimalHunt.bat added to the Primal Hunt folder. Run as administrator.
setlocal
set GAME=C:\Program Files (x86)\Fox\Aliens versus Predator 2 - Primal Hunt
net session >nul 2>&1 || (echo Run this as administrator. & pause & exit /b 1)
for %%f in (DDraw.dll D3DImm.dll dgVoodoo.conf d3d11.dll openxr_loader.dll xrres.exe "AVP2VR_PrimalHunt_(SteamVR_runtime).bat" avp2xr.ini avp2xr.ini.bak avp2xr.log) do if exist "%GAME%\%%~f" del "%GAME%\%%~f" && echo Removed %%~f
if exist "%GAME%\vrrez\cshell.dll" del "%GAME%\vrrez\cshell.dll" && echo Removed vrrez\cshell.dll
if exist "%GAME%\vrrez\Models" rmdir /s /q "%GAME%\vrrez\Models" && echo Removed vrrez\Models
if exist "%GAME%\vrrez" rmdir "%GAME%\vrrez" 2>nul
rem The copy of dgVoodoo.conf with the render resolution, which xrres.exe (run by the launcher) writes
set "VSCONF=%LOCALAPPDATA%\VirtualStore\Program Files (x86)\Fox\Aliens versus Predator 2 - Primal Hunt\dgVoodoo.conf"
if exist "%VSCONF%" del "%VSCONF%" && echo Removed the VirtualStore copy of dgVoodoo.conf
pause
rem Last, the copy of this script that the installer put in the game folder: (goto) ends the
rem batch first, so the copy can delete itself when that's the one being run
if exist "%GAME%\Uninstall_avp2vr_PrimalHunt.bat" (goto) 2>nul & del "%GAME%\Uninstall_avp2vr_PrimalHunt.bat"
