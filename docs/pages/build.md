\page build Build

\tableofcontents

\section build_with Built With

-   [SFML](https://github.com/SFML/SFML) - The Graphical library
-   [ImGui](https://github.com/ocornut/imgui) - The GUI library
-   [ImGui::SFML](https://github.com/eliasdaler/imgui-sfml) - The Binder library

\section for_windows For Windows

\subsection prerequisites Prerequisites

\subsubsection cpp_compiler C++ compiler

**MINGW 32**

1. Download with [MinGW 32 bits](https://osdn.net/projects/mingw/downloads/68260/mingw-get-setup.exe/).
2. After the download is complete, launch the program and wait for the download to finish.
3. When you get to the **MinGW Installation Manager** page, check all the boxes.
4. Then click on the **Installation** button then **Apply Changes**.

**MINGW 64**

1. Download with [MinGW 64 bits](https://sourceforge.net/projects/mingw-w64/files/Toolchains%20targetting%20Win32/Personal%20Builds/mingw-builds/installer/mingw-w64-install.exe/download).
2. Select the architecture **x86_64** and the treads to **posix**
3. Then click on the **Next** and wait until it's downloaded.

**ENV VARS**

You must then add the **environment variable** to your compiler:

1. Type **env** in the windows search bar and open **Edit the environment variables**.
2. Then click on **Environment variables** in **Avanced System Parameters**.
3. Click on the **Path** field then on **Edit**.
4. **Add** the path to the emplacement of the compiler: `...\Mingw\bin`

Check if it works by opening a new command line and typing:

```
> gcc

gcc: fatal error: no input files
compilation terminated.
```

\subsubsection cmake CMAKE

Install the cmake software that I use to controll the build process, and also build the libraries:

1. Go to http://www.cmake.org/download/ and click on the **Download button**.
2. Add it's path to then environment variables: `...\CMake\bin`

\subsection libraries Libraries

\subsubsection sfml SFML

1. Download precompiled library from https://www.sfml-dev.org/download/sfml/2.5.1/index-fr.php

Or compile it yourself:

1. Open a **new cmd** and type `git clone https://github.com/SFML/SFML.git`
2. Open **CMake** and put the path to the **SFML source code** into the **Where is the source code** field.
3. Create a **new folder** where to compile SFML
4. Put it's path into the **Where to build the binaries** field.
5. Click on the **Configure** button and select your c/c++ compiler
6. Click on **finish**.

> **To compile SFML in static**
>
> **Static:** don't need the .dll for the .exe to work
>
> 1. Uncheck **BUILD_SHARED_LIBS**
> 2. Check **SFML_USE_STATIC_STD_LIBS**
> 3. Edit the **CMake_INSTALL_PREFIX** to be the path of a new folder.
> 4. Click the **Configure** button once more to check there is no problem.
> 5. Then click on the **Generate** button.
> 6. Open a new command in the folder where SFML was built.
> 7. Run the following command: `mingw32-make install`

\subsubsection imgui IMGUI

1. Open a **new cmd** and type `git clone https://github.com/ocornut/imgui.git`
2. Go to the docking branch with `git checkout docking`

```
IMGUI_DIR = D:/Librairies/imgui
SFML_DIR = D:/Librairies/sfml_2_5_1/sfml_2_5_1_build/lib/cmake/SFML
SFML_DOC_DIR = D:/Librairies/sfml_2_5_1/sfml_2_5_1_build/lib/cmake/SFML
CMAKE_INSTALL_PREFIX = D:/Librairies/imgui_sfml
```

```
cmake . -D SFML_DIR="D:\Librairies\sfml_2_5_1/sfml_2_5_1_build\lib\cmake\SFML"
cmake D:\Librairies\imgui-sfml -DIMGUI_DIR=D:\Librairies\imgui -DSFML_DIR=D:\Librairies\sfml_2_5_1\sfml_2_5_1_build
```

3. Then open a new command in the folder where IMGUI was built.
4. Run the following command: `mingw32-make install`

\section for_linux For Linux

\warning todo complete this section

\section installation Installation

**To fetch the project files**

1. Clone this project with `git clone https://github.com/mlhoutel/Simulatio.git` <br/> _(or just click on the download button and extract the file where you want.)_

**[Structure details]**

<p>
The cloned project should contain these files:

```
├───.git
├───.vscode
├───assets          # Fonts, Libraries and Images
├───bin             # All binaries files
│   │───Demos       # Demos of functionnalities and performances
│   └───Saves       # Saved binaries of the precedent versions
=========================================================
├───build           # Where the project is compiled
│   │───Debug       # Debug binaries
│   └───Release     # Release binaries
=========================================================
├───docs            # Documentatiion and screenshots
├───include         # Headers files (.hpp)
└───src             # Sources files (.cpp)
```

</p>

**Build with the batch tool**

```
[0] Build/Run Debug
[1] Run Debug
[2] Build/Run Release
[3] Run Release
[4] Build Demos
[5] Build/Run Tests
[6] Build Docs
[7] Build Wrapper
[8] Sonar Scanner
[9] Exit
```

**In case of problem with the bat file, compile manually with**

```
cd build

and

cmake -G "MinGW Makefiles" -DCMAKE_BUILD_TYPE=Release ../.. && cmake --build .

or

cmake -G "MinGW Makefiles" -DCMAKE_BUILD_TYPE=Debug ../.. && cmake --build .
```

\section build_docs Build docs

1. Install doxygen (https://www.doxygen.nl/download.html)

**ENV VARS**

You must then add the **environment variable** to your compiler:

1. Type **env** in the windows search bar and open **Edit the environment variables**.
2. Then click on **Environment variables** in **Avanced System Parameters**.
3. Click on the **Path** field then on **Edit**.
4. **Add** the path to the emplacement of the compiler: `...\doxygen\bin`

Check if it works by opening a new command line and typing:

```
> doxygen

doxygen
error: Doxyfile not found and no input file specified!
Doxygen version 1.9.2 (caa4e3de211fbbef2c3adf58a6bd4c86d0eb7cb8)
Copyright Dimitri van Heesch 1997-2021
...
```
