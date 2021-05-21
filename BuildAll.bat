@ECHO OFF

set SOLUTION_FILE="Sources\Overload\Overload.sln"

IF exist %SOLUTION_FILE% (
    set INSTALLPATH=

    IF exist "%programfiles(x86)%\Microsoft Visual Studio\Installer\vswhere.exe" (
    for /F "tokens=* USEBACKQ" %%F in (`"%programfiles(x86)%\Microsoft Visual Studio\Installer\vswhere.exe" -version 16.0 -property installationPath`) do set INSTALLPATH=%%F
    )

    call "%INSTALLPATH%\Common7\Tools\VsDevCmd.bat"
    msbuild Sources\Overload\Overload.sln -m /p:Configuration=Debug
    msbuild Sources\Overload\Overload.sln -m /p:Configuration=Release
) ELSE (
    echo %SOLUTION_FILE% hasn't been found, have you run the "GenerateProjects.bat" script?
    PAUSE
)

