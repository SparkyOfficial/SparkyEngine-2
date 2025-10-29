@echo off
echo Copying shaders to build directory...
xcopy "%~dp0shaders" "%~dp0..\build_temp\Engine\shaders" /E /I /Y
echo Shaders copied successfully.