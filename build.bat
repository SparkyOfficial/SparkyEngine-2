@echo off
echo Building Sparky Engine...

REM Create build directory if it doesn't exist
if not exist "build" mkdir "build"

REM Change to build directory
cd build

REM Generate build files with CMake
cmake .. -G "Visual Studio 17 2022" -A x64

REM Build the project
cmake --build . --config Release

REM Check if build was successful
if %ERRORLEVEL% EQU 0 (
    echo.
    echo Build completed successfully!
    echo.
    echo To run the game, execute:
    echo   build\bin\Release\SparkyGame.exe
    echo.
    echo To run the JSON level test, execute:
    echo   build\bin\Release\test_json_level.exe
    echo.
    echo To run the components test, execute:
    echo   build\bin\Release\test_components.exe
) else (
    echo.
    echo Build failed with error level %ERRORLEVEL%
    echo Please check the error messages above.
)

pause