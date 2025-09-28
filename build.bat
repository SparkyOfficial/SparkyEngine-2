@echo off
echo Building Sparky Engine 2...
echo.

if not exist build mkdir build
cd build
cmake ..
cmake --build .

if %ERRORLEVEL% EQU 0 (
    echo.
    echo Build successful!
    echo.
    echo To run the game, execute:
    echo   cd Game
    echo   SparkyGame.exe
) else (
    echo.
    echo Build failed!
)
pause