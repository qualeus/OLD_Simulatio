@echo off
:menu
color 0a
cls
echo [0] Build/Run Debug 
echo [1] Run Debug 
echo [2] Build/Run Release
echo [3] Run Release
echo [4] Build Demos
echo [5] Build/Run Tests
echo [6] Build Docs
echo [7] Commit/Push Git
echo [8] Sonar Scanner
echo [9] Exit
set /p user=

if %user% == 0 goto build_debug
if %user% == 1 goto run_debug
if %user% == 2 goto build_release
if %user% == 3 goto run_release
if %user% == 4 goto build_demos
if %user% == 5 goto build_tests
if %user% == 6 goto build_docs
if %user% == 7 goto commit_push
if %user% == 8 goto sonar_scanner
if %user% == 9 exit

:build_debug
if not exist "build/Debug/" mkdir "build/Debug/"
cls
echo Compiling for Debug...
cd build\Debug
cmake -G "MinGW Makefiles" -DCMAKE_BUILD_TYPE=Debug ../.. && cmake --build .
pause
cd bin
Simulatio_Renderer.exe
if errorlevel 1 pause
cd ..\..\..
goto menu

:run_debug
cd build\Debug\bin
Simulatio_Renderer.exe
if errorlevel 1 pause
cd ..\..\..
goto menu

:build_release
if not exist "build/Release/" mkdir "build/Release/"
cls
echo Compiling for Release...
cd build\Release
cmake -G "MinGW Makefiles" -DCMAKE_BUILD_TYPE=Release ../.. && cmake --build .
cd bin
Simulatio_Renderer.exe
if errorlevel 1 pause
cd ..\..\..
goto menu

:run_release
cd build\Release\bin
Simulatio_Renderer.exe
if errorlevel 1 pause
cd ..\..\..
goto menu

:build_demos
goto menu

:build_tests
if not exist "build/Tests/" mkdir "build/Tests/"
cls
echo Compiling Tests...
cd build\Tests
cmake -G "MinGW Makefiles" -DCMAKE_BUILD_TYPE=Debug -DBUILD_TESTS=ON  ../.. && cmake --build .
mingw32-make CTEST_OUTPUT_ON_FAILURE=TRUE test
pause
cd ..\..
goto menu

:build_docs
if not exist "build/Docs/" mkdir "build/Docs/"
if not exist "docs/doxygen/" mkdir "docs/doxygen/"
cls
echo Building docs...
cd build\Docs
cmake -G "MinGW Makefiles" -DCMAKE_BUILD_TYPE=Debug -DBUILD_DOCS=ON  ../.. && cmake --build .
cd ..\..
pause
goto menu

:build_wrapper
if not exist "build/Debug/" mkdir "build/Debug/"
del /s /q "build/Debug/CMakeFiles"
rmdir /s /q "build/Debug/CMakeFiles"
cls
echo Compiling for Debug...
cd build\Debug
cmake -G "MinGW Makefiles" -DCMAKE_BUILD_TYPE=Debug ../.. 
build-wrapper --out-dir ../../bw-outputs cmake --build .
cd ..\..
goto menu

:sonar_scanner
sonar-scanner.bat \
  -D"sonar.organization=mlhoutel" \
  -D"sonar.projectKey=mlhoutel_Simulatio" \
  -D"sonar.sources=." \
  -D"sonar.cfamily.build-wrapper-output=bw-output" \
  -D"sonar.host.url=https://sonarcloud.io"
goto menu

:commit_push
cls
set /p message=Message du commit:
git add .
git commit -m "%message%"
git push
git pull
echo Done && pause >nul
goto menu
