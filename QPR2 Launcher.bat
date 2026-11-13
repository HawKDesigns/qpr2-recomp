@echo off
rem ATV: Quad Power Racing 2 recomp launcher.
rem   Double-click to open the launcher.
rem   "QPR2 Launcher.bat debug" keeps a console open and prints the launcher log.
setlocal
cd /d "%~dp0"
if /i "%~1"=="debug" (
    powershell.exe -NoProfile -ExecutionPolicy Bypass -File "%~dp0launcher\Launcher.ps1" -Console
    pause
) else (
    start "" powershell.exe -NoProfile -ExecutionPolicy Bypass -WindowStyle Hidden -File "%~dp0launcher\Launcher.ps1"
)
