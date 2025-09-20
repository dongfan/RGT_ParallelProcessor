@echo off
REM RGT Parallel Processor 빌드 및 실행 스크립트

echo ====================================
echo RGT Parallel Processor
echo ====================================

REM 현재 디렉토리 확인
echo Current directory: %CD%

REM GCC 설치 확인
echo Checking GCC installation...
g++ --version >nul 2>&1
if %ERRORLEVEL% neq 0 (
    echo ERROR: GCC not found! Please install MinGW-w64 or similar.
    pause
    exit /b 1
)

REM 컴파일
echo.
echo Compiling main.cpp...
g++ -std=c++17 -Wall -O2 -pthread -I./include -I./src src/main.cpp -o main.exe

if %ERRORLEVEL% neq 0 (
    echo ERROR: Compilation failed!
    pause
    exit /b 1
)

echo Compilation successful!

REM 실행
echo.
echo Running the program...
echo ====================================
main.exe

echo.
echo ====================================
echo Program finished.
pause