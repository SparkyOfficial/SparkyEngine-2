@echo off
echo Building SparkyEngine2...
echo ========================

REM Create build directory if it doesn't exist
if not exist "build" mkdir build

REM Navigate to build directory
cd build

REM Configure with CMake (audio disabled)
echo Configuring with CMake...
cmake .. -DENABLE_AUDIO=OFF

REM Build the project
echo Building project...
cmake --build . --config Release

REM Check if build was successful
if %ERRORLEVEL% EQU 0 (
    echo.
    echo Build successful!
    echo.
    echo Running the game...
    echo =================
    cd Game\Release
    start SparkyGame.exe
    cd ..\..
) else (
    echo.
    echo Build failed with error level %ERRORLEVEL%
    echo Please check the error messages above
)

echo.
echo Returning to project root...
cd ..