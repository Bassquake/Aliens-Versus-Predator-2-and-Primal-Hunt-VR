@echo off
rem Copies dgVoodoo2 and the avp2xr D3D11/OpenXR hook into the Primal Hunt folder, as Install_avp2vr.bat does
rem for AVP2. Run as administrator. The Primal Hunt client (deploy\vrrez_ph\cshell.dll, built with
rem /p:PrimalHunt=1) goes in that folder's vrrez, without cres.dll or sres.dll: Primal Hunt's own have its text.
setlocal
set GAME=C:\Program Files (x86)\Fox\Aliens versus Predator 2 - Primal Hunt
net session >nul 2>&1 || (echo Run this as administrator. & pause & exit /b 1)
for %%f in (DDraw.dll D3DImm.dll dgVoodoo.conf d3d11.dll openxr_loader.dll xrres.exe "AVP2VR_PrimalHunt_(SteamVR_runtime).bat" Uninstall_avp2vr_PrimalHunt.bat) do (
	copy /y "%~dp0deploy\%%~f" "%GAME%\" >nul || (echo Failed to copy %%~f & pause & exit /b 1)
	echo Installed %%~f
)
rem Settings: deploy\avp2xr.ini is the one to edit; the game folder's previous copy is kept as .bak
if exist "%GAME%\avp2xr.ini" copy /y "%GAME%\avp2xr.ini" "%GAME%\avp2xr.ini.bak" >nul
copy /y "%~dp0deploy\avp2xr.ini" "%GAME%\" >nul || (echo Failed to copy avp2xr.ini & pause & exit /b 1)
echo Installed avp2xr.ini
if not exist "%GAME%\vrrez" mkdir "%GAME%\vrrez"
copy /y "%~dp0deploy\vrrez_ph\cshell.dll" "%GAME%\vrrez\" >nul || (echo Failed to copy vrrez\cshell.dll & pause & exit /b 1)
echo Installed vrrez\cshell.dll
rem The same model fixes for VR as AVP2's: Primal Hunt uses AVP2's models for these weapons
if exist "%~dp0deploy\vrrez\Models" (
	xcopy /s /i /y /q "%~dp0deploy\vrrez\Models" "%GAME%\vrrez\Models" >nul || (echo Failed to copy vrrez\Models & pause & exit /b 1)
	echo Installed vrrez\Models
)
pause
