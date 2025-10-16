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

echo Compiling skybox vertex shader...
%VULKAN_SDK%\Bin\glslc.exe skybox.vert -o skybox.vert.spv
if %ERRORLEVEL% EQU 0 (
    echo Skybox vertex shader compiled successfully!
) else (
    echo Failed to compile skybox vertex shader!
    exit /b %ERRORLEVEL%
)

echo Compiling material vertex shader...
%VULKAN_SDK%\Bin\glslc.exe material.vert -o material.vert.spv
if %ERRORLEVEL% EQU 0 (
    echo Material vertex shader compiled successfully!
) else (
    echo Failed to compile material vertex shader!
    exit /b %ERRORLEVEL%
)

echo Compiling PBR vertex shader...
%VULKAN_SDK%\Bin\glslc.exe pbr.vert -o pbr.vert.spv
if %ERRORLEVEL% EQU 0 (
    echo PBR vertex shader compiled successfully!
) else (
    echo Failed to compile PBR vertex shader!
    exit /b %ERRORLEVEL%
)

echo Compiling advanced PBR vertex shader...
%VULKAN_SDK%\Bin\glslc.exe advanced_pbr.vert -o advanced_pbr.vert.spv
if %ERRORLEVEL% EQU 0 (
    echo Advanced PBR vertex shader compiled successfully!
) else (
    echo Failed to compile advanced PBR vertex shader!
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

echo Compiling skybox fragment shader...
%VULKAN_SDK%\Bin\glslc.exe skybox.frag -o skybox.frag.spv
if %ERRORLEVEL% EQU 0 (
    echo Skybox fragment shader compiled successfully!
) else (
    echo Failed to compile skybox fragment shader!
    exit /b %ERRORLEVEL%
)

echo Compiling material fragment shader...
%VULKAN_SDK%\Bin\glslc.exe material.frag -o material.frag.spv
if %ERRORLEVEL% EQU 0 (
    echo Material fragment shader compiled successfully!
) else (
    echo Failed to compile material fragment shader!
    exit /b %ERRORLEVEL%
)

echo Compiling PBR fragment shader...
%VULKAN_SDK%\Bin\glslc.exe pbr.frag -o pbr.frag.spv
if %ERRORLEVEL% EQU 0 (
    echo PBR fragment shader compiled successfully!
) else (
    echo Failed to compile PBR fragment shader!
    exit /b %ERRORLEVEL%
)

echo Compiling advanced PBR fragment shader...
%VULKAN_SDK%\Bin\glslc.exe advanced_pbr.frag -o advanced_pbr.frag.spv
if %ERRORLEVEL% EQU 0 (
    echo Advanced PBR fragment shader compiled successfully!
) else (
    echo Failed to compile advanced PBR fragment shader!
    exit /b %ERRORLEVEL%
)

echo All shaders compiled successfully!
pause