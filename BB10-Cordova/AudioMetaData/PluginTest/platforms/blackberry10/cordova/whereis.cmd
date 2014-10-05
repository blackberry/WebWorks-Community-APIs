@echo off
setlocal enabledelayedexpansion
set var_a=%1
call :sub %var_a%
if exist %var_b% if not exist %var_b%\nul goto exit
for %%i in (.com .exe .cmd .bat) do (
  call :sub %var_a%%%i
  if exist !var_b! goto exit
)
echo INFO: could not find files for the given pattern(s) 1>&2
exit /b 1

:sub
set var_b=%~$PATH:1
goto :EOF

:exit
echo %var_b%
exit /b 0
