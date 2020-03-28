@echo off
color 0a
set sfmlPath="D:\Librairies\C\SFML-2.5.1-Static\SFML"

:menu
cls
echo [1] Compile debug
echo [2] Compile and run debug 
echo [3] Run debug 
echo [4] Compile release 
echo [5] Compile and run release
echo [6] Run release
echo [7] Commit and Push Git
echo [8] Exit
set /p user=

if %user% == 1 goto cDebug
if %user% == 2 goto crDebug
if %user% == 3 goto rDebug
if %user% == 4 goto cRelease
if %user% == 5 goto crRelease
if %user% == 6 goto rRelease
if %user% == 7 goto Git
if %user% == 8 exit

:cDebug
cls
echo Compiling for debug...
g++ -o bin/debug/*.o -c source/*.cpp -std=c++14 -pedantic -Wall -Wextra -Wold-style-cast -Woverloaded-virtual -Wfloat-equal -Wwrite-strings -Wpointer-arith -Wcast-qual -Wcast-align -Wconversion -Wshadow -Weffc++ -Wredundant-decls -Wdouble-promotion -Winit-self -Wswitch-default -Wswitch-enum -Wundef -Wlogical-op -Winline -DSFML_STATIC -I %sfmlPath%\include
g++ bin/debug/*.o -o bin/debug/Debug.exe -L %sfmlPath%\lib -lsfml-graphics-s -lsfml-window-s -lsfml-system-s -lopengl32 -lwinmm -lgdi32 -lfreetype
echo Done && pause >nul
goto menu

:crDebug
cls
echo Compiling for debug...
g++ -o bin/debug/*.o -c source/*.cpp -std=c++14 -Werror -Wfatal-errors -DSFML_STATIC -I %sfmlPath%\include
g++ bin/debug/*.o -o bin/debug/Debug.exe -L %sfmlPath%\lib -lsfml-graphics-s -lsfml-window-s -lsfml-system-s -lopengl32 -lwinmm -lgdi32 -lfreetype
echo Done
"bin/debug/Debug.exe"
goto menu

:rDebug
cls
"bin/debug/Debug.exe"
goto menu

:cRelease
cls
echo Compiling for release...
g++ -o bin/release/*.o -c source/*.cpp -std=c++14 -Werror -Wfatal-errors -DSFML_STATIC -I %sfmlPath%\include -O3 -s -fexpensive-optimizations
g++ bin/release/*.o -o bin/release/Release.exe -L %sfmlPath%/lib -lsfml-graphics-s -lsfml-window-s -lsfml-system-s -lopengl32 -lwinmm -lgdi32 -lfreetype
echo Done && pause >nul
goto menu

:crRelease
cls
echo Compiling for release...
g++ -o bin/release/*.o -c source/*.cpp -std=c++14 -Werror -Wfatal-errors -DSFML_STATIC -I %sfmlPath%\include -O3 -s -fexpensive-optimizations
g++ bin/release/*.o -o bin/release/Release.exe -L %sfmlPath%\lib -lsfml-graphics-s -lsfml-window-s -lsfml-system-s -lopengl32 -lwinmm -lgdi32 -lfreetype
echo Done
"bin/release/Release.exe"
goto menu

:rRelease
cls
"bin/release/Release.exe"
goto menu

:Git
cls
set /p message=Message du commit:
git add .
git commit -m "%message%"
git push
git pull
echo Done && pause >nul
goto menu
