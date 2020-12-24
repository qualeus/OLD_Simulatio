@echo off
set lib_path="D:\Documents\Projets\Physics\ressources\Libraries"

:menu
color 0a
cls
echo [1] Build/Run Debug 
echo [2] Run Debug 
echo [3] Build/Run Release
echo [4] Run Release
echo [5] Build Demos
echo [6] Build Tests
echo [7] Commit/Push Git
echo [8] Exit
set /p user=

if %user% == 1 goto build_debug
if %user% == 2 goto run_debug
if %user% == 3 goto build_release
if %user% == 4 goto run_release
if %user% == 5 goto build_demos
if %user% == 6 goto build_tests
if %user% == 7 goto commit_push
if %user% == 8 exit

:build_debug
if not exist "build/Debug/" mkdir "build/Debug/"
cls
echo Compiling for Debug...
cd build\Debug
cmake -G "MinGW Makefiles" -DCMAKE_BUILD_TYPE=Debug ../.. && cmake --build .
cd bin
physics.exe
cd ..\..\..
goto menu

:run_debug
cd build\Debug\bin
physics.exe
cd ..\..\..
goto menu

:build_release
if not exist "build/Release/" mkdir "build/Release/"
cls
echo Compiling for Release...
cd build\Release
cmake -G "MinGW Makefiles" -DCMAKE_BUILD_TYPE=Release ../.. && cmake --build .
cd bin
physics.exe
cd ..\..\..
goto menu

:run_release
cd build\Release\bin
physics.exe
cd ..\..\..
goto menu

:build_demos
goto menu

:build_tests
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
