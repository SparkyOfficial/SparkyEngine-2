@echo off
echo Starting Sparky Engine 2...
echo.

if exist build\Game\SparkyGame.exe (
    cd build\Game
    SparkyGame.exe
) else (
    echo Game executable not found. Please build the project first.
    echo Run build.bat to build the project.
)
pause