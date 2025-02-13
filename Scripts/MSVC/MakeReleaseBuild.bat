pushd %~dp0
call .\BuildAll.bat Debug
call .\BuildAll.bat Release
popd

:: The output is located in a folder called ..\..\Build\Release. Create an archive with this folder's content, and name the archive Overload-<version>-<platform>.zip.
for /f "delims=" %%v in (..\..\VERSION.txt) do set version=%%v
set platform=win32_x64

:: Navigate to the release folder
pushd ..\..\Build\Release

:: Ensure the Releases folder exists
mkdir ..\..\Releases

:: Create the archive
powershell Compress-Archive -Path * -DestinationPath ..\..\Releases\Overload-%version%-%platform%.zip

:: Return to the original directory
popd