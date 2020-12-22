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
cls
echo Compiling for Debug...

for /R %%f in (src/*.cpp) do (
	g++ -o bin/Source/Debug/%%~nf.o -c src/%%~nxf -std=c++17 -Werror -Wfatal-errors -DSFML_STATIC -I %lib_path%\include
	IF NOT %ERRORLEVEL%==0 (pause && goto menu)
)
IF %ERRORLEVEL%==0 (
	g++ bin/Source/Debug/*.o -o bin/Source/Debug/Debug.exe -L %lib_path%\lib -lsfml-graphics-s -lsfml-window-s -lsfml-system-s -lopengl32 -lwinmm -lgdi32 -lfreetype
	xcopy /f /y "ressources/Fonts/arial.ttf" "bin/Source/Debug/"
	goto run_debug
)
goto menu

:run_debug
cls
cd "bin/Source/Debug/"
"Debug.exe"
cd "../../../"
goto menu

:build_release
cls
echo Compiling for Release...

for /R %%f in (src/*.cpp) do (
	g++ -o bin/Source/Release/%%~nf.o -c src/%%~nxf -std=c++17 -Werror -Wfatal-errors -DSFML_STATIC -I %lib_path%\include -O3 -s -fexpensive-optimizations
	IF NOT %ERRORLEVEL%==0 (pause && goto menu)
)
IF %ERRORLEVEL%==0 (
	g++ bin/Source/Release/*.o -o bin/Source/Release/Release.exe -L %lib_path%\lib -lsfml-graphics-s -lsfml-window-s -lsfml-system-s -lopengl32 -lwinmm -lgdi32 -lfreetype
	xcopy /f /y "ressources/Fonts/arial.ttf" "bin/Source/Release/"
	goto run_release
)
goto menu

:run_release
cls
cd "bin/Source/Release/"
"Release.exe"
cd "../../../"
goto menu

:build_demos
cls
echo Compiling Demos...

for /R %%f in (src/Demos/*.cpp) do (
	if not exist "bin/Demos/Debug/%%~nf/" mkdir "bin/Demos/Debug/%%~nf/"
	if not exist "bin/Demos/Release/%%~nf/" mkdir "bin/Demos/Release/%%~nf/"

	for /R %%s in (src/*.cpp) do (
		if not "%%~nxs"=="main.cpp" (
			g++ -o bin/Demos/Debug/%%~nf/%%~ns.o -c src/%%~nxs -std=c++17 -Werror -Wfatal-errors -DSFML_STATIC -I %lib_path%\include
			g++ -o bin/Demos/Release/%%~nf/%%~ns.o -c src/%%~nxs -std=c++17 -Werror -Wfatal-errors -DSFML_STATIC -I %lib_path%\include -O3 -s -fexpensive-optimizations
			echo %%~nxs - %%~nxf Done
		)
	)

	g++ -o bin/Demos/Debug/%%~nf/%%~nf.o -c src/Demos/%%~nxf -std=c++17 -Werror -Wfatal-errors -DSFML_STATIC -I %lib_path%\include
	g++ bin/Demos/Debug/%%~nf/*.o -o bin/Demos/Debug/%%~nf/%%~nf.exe -L %lib_path%\lib -lsfml-graphics-s -lsfml-window-s -lsfml-system-s -lopengl32 -lwinmm -lgdi32 -lfreetype

	g++ -o bin/Demos/Release/%%~nf/%%~nf.o -c src/Demos/%%~nxf -std=c++17 -Werror -Wfatal-errors -DSFML_STATIC -I %lib_path%\include -O3 -s -fexpensive-optimizations
	g++ bin/Demos/Release/%%~nf/*.o -o bin/Demos/Release/%%~nf/%%~nf.exe -L %lib_path%\lib -lsfml-graphics-s -lsfml-window-s -lsfml-system-s -lopengl32 -lwinmm -lgdi32 -lfreetype

	xcopy /f /y "ressources/Fonts/arial.ttf" "bin/Demos/Debug/%%~nf/"
	xcopy /f /y "ressources/Fonts/arial.ttf" "bin/Demos/Release/%%~nf/"

	echo %%~nxf Done
)
pause
goto menu

:build_tests
cls
echo Compiling Tests...

for /R %%f in (src/Tests/*.cpp) do (
	if not exist "bin/Tests/Debug/%%~nf/" mkdir "bin/Tests/Debug/%%~nf/"
	if not exist "bin/Tests/Release/%%~nf/" mkdir "bin/Tests/Release/%%~nf/"

	for /R %%s in (src/*.cpp) do (
		if "%%~nxs"=="Functional.cpp" (
			g++ -o bin/Tests/Debug/%%~nf/%%~ns.o -c src/%%~nxs -std=c++17 -Werror -Wfatal-errors -DSFML_STATIC -I %lib_path%\include
			g++ -o bin/Tests/Release/%%~nf/%%~ns.o -c src/%%~nxs -std=c++17 -Werror -Wfatal-errors -DSFML_STATIC -I %lib_path%\include -O3 -s -fexpensive-optimizations
			echo %%~nxs - %%~nxf Done
		)
	)

	g++ -o bin/Tests/Debug/%%~nf/%%~nf.o -c src/Tests/%%~nxf -std=c++17 -Werror -Wfatal-errors -DSFML_STATIC -I %lib_path%\include
	g++ bin/Tests/Debug/%%~nf/*.o -o bin/Tests/Debug/%%~nf/%%~nf.exe -L %lib_path%\lib -lsfml-graphics-s -lsfml-window-s -lsfml-system-s -lopengl32 -lwinmm -lgdi32 -lfreetype

	g++ -o bin/Tests/Release/%%~nf/%%~nf.o -c src/Tests/%%~nxf -std=c++17 -Werror -Wfatal-errors -DSFML_STATIC -I %lib_path%\include -O3 -s -fexpensive-optimizations
	g++ bin/Tests/Release/%%~nf/*.o -o bin/Tests/Release/%%~nf/%%~nf.exe -L %lib_path%\lib -lsfml-graphics-s -lsfml-window-s -lsfml-system-s -lopengl32 -lwinmm -lgdi32 -lfreetype

	echo %%~nxf Done
)
pause
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
