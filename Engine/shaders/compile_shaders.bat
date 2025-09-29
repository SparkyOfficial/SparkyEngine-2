@echo off
echo Compiling shaders...

REM Set the path to the Vulkan SDK
set VULKAN_SDK=E:\VulkanSDK\1.4.321.1

REM Compile vertex shaders
echo Compiling advanced vertex shader...
%VULKAN_SDK%\Bin\glslc.exe advanced.vert -o advanced.vert.spv
if %ERRORLEVEL% EQU 0 (
    echo Advanced vertex shader compiled successfully!
) else (
    echo Failed to compile advanced vertex shader!
    exit /b %ERRORLEVEL%
)

REM Compile fragment shaders
echo Compiling advanced fragment shader...
%VULKAN_SDK%\Bin\glslc.exe advanced.frag -o advanced.frag.spv
if %ERRORLEVEL% EQU 0 (
    echo Advanced fragment shader compiled successfully!
) else (
    echo Failed to compile advanced fragment shader!
    exit /b %ERRORLEVEL%
)

echo All shaders compiled successfully!
pause