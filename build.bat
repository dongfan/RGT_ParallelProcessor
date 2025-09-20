@echo off
REM CMake 빌드 스크립트

echo ====================================
echo RGT Parallel Processor - CMake Build
echo ====================================

REM 빌드 디렉토리 생성
if not exist "build" (
    echo Creating build directory...
    mkdir build
)

cd build

REM CMake 설정
echo Configuring with CMake...
cmake .. -DCMAKE_BUILD_TYPE=Release

if %ERRORLEVEL% neq 0 (
    echo ERROR: CMake configuration failed!
    cd ..
    pause
    exit /b 1
)

REM 빌드
echo Building...
cmake --build . --config Release

if %ERRORLEVEL% neq 0 (
    echo ERROR: Build failed!
    cd ..
    pause
    exit /b 1
)

REM 실행
echo.
echo Running the program...
echo ====================================

if exist "Release\main.exe" (
    Release\main.exe
) else if exist "main.exe" (
    main.exe
) else (
    echo ERROR: Executable not found!
    cd ..
    pause
    exit /b 1
)

echo.
echo ====================================
echo Program finished.
cd ..
pause