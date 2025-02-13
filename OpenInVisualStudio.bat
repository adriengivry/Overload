@echo off

REM Call the GenerateProjects script
call %~dp0\Scripts\GenerateProjects.bat

REM Check if the project generation was successful
if %errorlevel% neq 0 (
    echo Project generation failed. Exiting.
    exit /b %errorlevel%
)

REM Call the OpenSolution script
call %~dp0\Scripts\MSVC\OpenSolution.bat

exit /b 0
