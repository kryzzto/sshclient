@echo off

REM Check if CMake is installed
where cmake >nul 2>nul
if errorlevel 1 (
    echo CMake is not installed. Please install it from https://cmake.org/download/.
    pause
    exit /b
)

REM Check if Visual Studio is installed
where "MSBuild.exe" >nul 2>nul
if errorlevel 1 (
    echo Visual Studio is not installed. Please install it from https://visualstudio.microsoft.com/downloads/.
    pause
    exit /b
)

REM Create build directory if it doesn't exist
if not exist build mkdir build
cd build

REM Run cmake to configure the project and download libssh
cmake .. -G "Visual Studio 17 2022" -A x64

REM Build the project
cmake --build . --config Release

echo Build completed!
pause
