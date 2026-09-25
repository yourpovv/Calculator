@echo off
setlocal
mkdir build 2>nul

where cl >nul 2>&1
if not errorlevel 1 (
    echo Building with MSVC...
    rc /nologo /fo build\app.res app.rc
    if errorlevel 1 exit /b %errorlevel%
    cl /std:c++17 /EHsc /nologo /Iinclude src\calculator.cpp src\banner.cpp src\terminal.cpp src\main.cpp build\app.res /Fo:build\ /Fe:build\calculator.exe
    exit /b %errorlevel%
)

where g++ >nul 2>&1
if not errorlevel 1 (
    echo Building with g++...
    if not exist assets\icon.ico (
        echo Missing assets\icon.ico required by app.rc
        exit /b 1
    )
    where windres >nul 2>&1
    if errorlevel 1 (
        echo MinGW windres is required to embed app metadata
        exit /b 1
    )
    windres -DMINGW_BUILD app.rc -O coff -o build\app.res
    if errorlevel 1 exit /b %errorlevel%
    g++ -static -std=c++17 -Wall -Wextra -Iinclude src\calculator.cpp src\banner.cpp src\terminal.cpp src\main.cpp build\app.res -o build\calculator.exe
    exit /b %errorlevel%
)

echo MSVC Build Tools/MinGW not installed
exit /b 1
