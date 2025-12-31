@echo off
echo ================================
echo Generating Visual Studio 2022 solution
echo ================================

REM Move to the directory of this script
cd /d "%~dp0"

REM Check if premake5.exe exists
if not exist premake5.exe (
    echo ERROR: premake5.exe not found in this directory.
    pause
    exit /b 1
)

REM Run Premake
premake5 vs2022

if %errorlevel% neq 0 (
    echo.
    echo Premake failed!
    pause
    exit /b %errorlevel%
)

echo.
echo Solution generated successfully!
echo You can now open OpenGLRenderer.sln
pause
