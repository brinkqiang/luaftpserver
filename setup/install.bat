
cd %~dp0
set CURRENT_DIR=%~dp0
set CURRENT_FILE=%~dp0\lua.exe

nssm install luaftpserver "%CURRENT_FILE%" ../script/main.lua
nssm set luaftpserver AppDirectory %CURRENT_DIR%
nssm dump luaftpserver
nssm start luaftpserver
pause