@echo off
setlocal enabledelayedexpansion

:: Get the configuration parameter
set "CONFIGURATION=%~1"

:: Build the solution
if "%CONFIGURATION%"=="" (
    echo Launching Overload Editor in Debug mode...
    start %~dp0\..\Build\Debug\OvEditor.exe
) else (
    echo Launching Overload Editor in %CONFIGURATION% mode...
    start %~dp0\..\Build\%CONFIGURATION%\OvEditor.exe
)

if %errorlevel% neq 0 (
    echo Launching the editor failed.
    exit /b %errorlevel%
)

echo Editor launched successfully.
