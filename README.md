# Physics

This project is a personal challenge that I launched to train me in programming and compiling the c++ language, while learning the basic concepts and functions of 2D physical simulation (applicable to 3D thereafter).

The ultimate goal would be to present an easy-to-use and optimized physical engine, a bit like a simplified "Algodoo"

## Getting Started

In order to run this c ++ program using the SFML graphics library, you will need to meet the conditions for its operation. 

### Prerequisites

To install a c/c++ compilator:

If you are under Windows, I recommend the mingw compiler:
Go to the http://www.mingw.org/ website and click on the Download button. After the download is complete, launch the program and wait for the download to finish. When you get to the "MinGW Installation Manager" page, check all the boxes, then click on the "installation" button and "Apply Changes".

You must then add the environment variable to your compiler:
search for it in the windows search bar and open it, then click on it again in "avanced system parameters". Click on the "Path" field and click on "Edit". You can then "Add" the path to the emplacement of the compiler: Path\to\Compiler\Mingw\bin.

You can try if it worked by writing in a new command
```
gcc
```
If the return is the following, this part work.
```
gcc: fatal error: no input files
compilation terminated.
```

To install the SFML library:
Go to the https://www.sfml-dev.org/download/sfml/2.5.1/index-fr.php website and choose a download link.

If you want to be sure it will work, you can compile yourself the library from the source-code in the page or at https://github.com/SFML/SFML. Then extract it in a new folder.

You can install CMake to facilitate the process with a graphical interface (https://cmake.org/download/), or you can just use the command line.

Open CMake and put the path to the extracted SFML folder into the "Where is the source code" field. Create a new folder to put your compiled SFML and put it's path into the "Where to build the binaries" field.

Click on the "Configure" button and select your c/c++ compiler (if you installed MinGW, select MinGW Makefiles) and click on finish.

To compile SFML in static (all in once, don't need the .dll for the .exe to work) uncheck "BUILD_SHARED_LIBS" and check "SFML_USE_STATIC_STD_LIBS" and edit the "CMake_INSTALL_PREFIX" to be the path of a new folder with the final library files we will use. You can then click the "Configure" button once more to check there is no problem, then click on the "Generate" button.

Then open a new command and go to the folder where SFML was built. You can now run the following command:
```
mingw32-make install
```
If there is no problem during this process, the SFML library should be working.

### Installing

Now, you can download or clone this git where you want.

```
git clone https://github.com/mlhoutel/Physics.git
```
or just click on the download button and extract the file where you want.

If you want to use the automatic compilation by command, open the "build.bat" file in a text editor like Notepad and edit this line to match your SMFL compilated folder:
```
sfmlPath=Path\to\SFML\SFML-2.5.1-Static\SFML
```
It should contain these files:
```
├───bin
├───include
│   └───SFML
│       ├───Audio
│       ├───Graphics
│       ├───Network
│       ├───System
│       └───Window
└───lib
    └───cmake
        └───SFML
```
You can now run the build.bat file and press 5 to compile and run the demo code.

## Built With

* [SFML](https://www.sfml-dev.org/tutorials/2.5/) - The graphical library

## Contributing

Please read [CONTRIBUTING.md]() for details on our code of conduct, and the process for submitting pull requests to us.

## Authors

* **LHOUTELLIER Maël** - *Initial work* - [mlhoutel](https://github.com/mlhoutel)

See also the list of [contributors](https://github.com/your/project/contributors) who participated in this project.

## License

This project is licensed under the MIT License - see the [LICENSE.md](LICENSE.md) file for details

