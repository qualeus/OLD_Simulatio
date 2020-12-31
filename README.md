# Physics

This project started from the personal challenge of recreating physical phenomena in a simulation running in real time.

The goal was to learn more about the functioning of these topics, such as:
- **Gravity** (My goal was not to model objects at the molecular level so I used the classical mecanics one)
- **Collisions** (3rd law of Newton/Reciprocal force between two bodies)
- **Friction** (Approximation with dissipation in heat)
- **Fluids** (IN DEV)
- **Electromagnetic fields** (IN DEV)

This project also allows me to familiarize myself with several areas of programming, like:
- **Graph Theory** and advanced **Data structures** like Quadtrees
- **C++ programs/libraries** compilation (Mingw batch/ CMake)
- **Memory allocation** and **Classes Inheritances**
- **Optimisation** 
- **Threads** and Graphical acceleration with CUDA (IN DEV)

> To summarize this is an introduction to the basic concepts and functions of 2D physical simulation (applicable to 3D thereafter).

The ultimate goal would be to present an easy-to-use and optimized physical engine, a bit like a simplified "Algodoo".

---

## Prerequisites

### CPP compiler

**MINGW 32**
 * Download with [MinGW 32 bits](https://osdn.net/projects/mingw/downloads/68260/mingw-get-setup.exe/). 
 * After the download is complete, launch the program and wait for the download to finish. 
 * When you get to the **MinGW Installation Manager** page, check all the boxes.
 * Then click on the **Installation** button then **Apply Changes**.

**MINGW 64**
* Download with [MinGW 64 bits](https://sourceforge.net/projects/mingw-w64/files/Toolchains%20targetting%20Win32/Personal%20Builds/mingw-builds/installer/mingw-w64-install.exe/download). 
 * Select the architecture **x86_64** and the treads to **posix**
 * Then click on the **Next** and wait until it's downloaded.

You must then add the **environment variable** to your compiler:
 * Type **env** in the windows search bar and open **Edit the environment variables**.
 * Then click on **Environment variables** in **Avanced System Parameters**. 
 * Click on the **Path** field then on **Edit**. 
 * **Add** the path to the emplacement of the compiler: ```...\Mingw\bin```

Check if it works by opening a new command line and typing:
```
> gcc

gcc: fatal error: no input files
compilation terminated.
```
### CMAKE

Install the cmake software that I use to controll the build process, and also build the libraries:
 * Go to http://www.cmake.org/download/ and click on the **Download button**.
 * Add it's path to then environment variables: ```...\CMake\bin```

---

## Libraries

### SFML

Download precompiled library from https://www.sfml-dev.org/download/sfml/2.5.1/index-fr.php

**OR**

Compile it yourself:

* Open a **new cmd** and type ```git clone https://github.com/SFML/SFML.git```
* Open **CMake** and put the path to the **SFML source code** into the **Where is the source code** field. 
* Create a **new folder** where to compile SFML 
* Put it's path into the **Where to build the binaries** field.
* Click on the **Configure** button and select your c/c++ compiler
* Click on **finish**.

*To compile SFML in static (all in once, don't need the .dll for the .exe to work)*

* Uncheck **BUILD_SHARED_LIBS**
* Check **SFML_USE_STATIC_STD_LIBS**
* Edit the **CMake_INSTALL_PREFIX** to be the path of a new folder.

* Click the **Configure** button once more to check there is no problem.
* Then click on the **Generate** button.

Then open a new command in the folder where SFML was built.

 * Run the following command: ```mingw32-make install```

### IMGUI

IMGUI_DIR = D:/Librairies/imgui
SFML_DIR = D:/Librairies/sfml_2_5_1/sfml_2_5_1_build/lib/cmake/SFML
SFML_DOC_DIR = D:/Librairies/sfml_2_5_1/sfml_2_5_1_build/lib/cmake/SFML
CMAKE_INSTALL_PREFIX = D:/Librairies/imgui_sfml

```
cmake . -D SFML_DIR="D:\Librairies\sfml_2_5_1/sfml_2_5_1_build\lib\cmake\SFML"
cmake D:\Librairies\imgui-sfml -DIMGUI_DIR=D:\Librairies\imgui -DSFML_DIR=D:\Librairies\sfml_2_5_1\sfml_2_5_1_build
```

Then open a new command in the folder where IMGUI was built.

 * Run the following command: ```mingw32-make install```

## Installing

Download this repo with ```git clone https://github.com/mlhoutel/Physics.git```

*(or just click on the download button and extract the file where you want.)*

It should contain these files:
```
├───.git
├───bin             # All binaries files
│   │───Demos       # Demos of functionnalities and performances (ex: \Release\granular_demo\granular_demo.exe)
│   │───Saves       # Saved binaries of the last versions (ex: \v0.2.3\Release)
│   │───Sources     # Where the current build is stored (Debug and Release)
│   └───Tests       # Manuals Units tests (ex: \Release\functional_computations\functional_computations.exe)
├───include         # Headers files (.hpp)
├───ressources      # Fonts, Libraries and Images
└───src             # Sources files (.cpp)
    │───Demos       # Demos files
    └───Tests       # Tests files
```

~~If you want to use the automatic compilation by command, open the "build.bat" file in a text editor like Notepad and edit this line to match your SMFL compilated folder: ``` set lib_path="D:\Projets\Physics\ressources\Libraries"```.
You can now run the build.bat file and press 5 to compile and run the demo code.~~

```
cd build
cmake -G "MinGW Makefiles" -DCMAKE_BUILD_TYPE=Release ../.. && cmake --build .
cmake -G "MinGW Makefiles" -DCMAKE_BUILD_TYPE=Debug ../.. && cmake --build .
```

---

## Built With

* [SFML](https://github.com/SFML/SFML) - The Graphical library
* [ImGui](https://github.com/ocornut/imgui) - The GUI library
* [ImGui::SFML](https://github.com/eliasdaler/imgui-sfml) - The Binder library

## Contributing

Please read [CONTRIBUTING.md](CONSTRIBUTING.md) for details on our code of conduct, and the process for submitting pull requests to us.

## Authors

* **LHOUTELLIER Maël** - *Initial work* - [mlhoutel](https://github.com/mlhoutel)

## License

This project is licensed under the MIT License - see the [LICENSE.md](LICENSE.md) file for details

