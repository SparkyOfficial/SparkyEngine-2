@echo off
echo Building Sparky Engine 3D Game...

REM Clean previous build
if exist "build" rmdir /s /q "build"

REM Create build directory
mkdir build
cd build

REM Configure with CMake
cmake .. -G "Visual Studio 17 2022" -A x64

REM Build the project
cmake --build . --config Release

echo.
echo Build complete!
echo Executable will be in build\bin\Release\ directory
pause