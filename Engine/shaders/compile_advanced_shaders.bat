@echo off
echo Compiling Advanced Shaders...

REM Set the path to the Vulkan SDK
set VULKAN_SDK_PATH=E:\VulkanSDK\1.4.321.1

REM Check if Vulkan SDK is available
if not exist "%VULKAN_SDK_PATH%" (
    echo Vulkan SDK not found at %VULKAN_SDK_PATH%
    echo Please update the VULKAN_SDK_PATH variable in this script
    pause
    exit /b 1
)

REM Compile vertex shaders
echo Compiling advanced PBR vertex shader...
"%VULKAN_SDK_PATH%\Bin\glslangValidator.exe" -V advanced_pbr.vert -o advanced_pbr.vert.spv
if %ERRORLEVEL% NEQ 0 (
    echo Failed to compile advanced_pbr.vert
    pause
    exit /b %ERRORLEVEL%
)

REM Compile fragment shaders
echo Compiling advanced PBR fragment shader...
"%VULKAN_SDK_PATH%\Bin\glslangValidator.exe" -V advanced_pbr.frag -o advanced_pbr.frag.spv
if %ERRORLEVEL% NEQ 0 (
    echo Failed to compile advanced_pbr.frag
    pause
    exit /b %ERRORLEVEL%
)

echo Shader compilation completed successfully!
pause