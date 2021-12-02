\mainpage Simulatio

\tableofcontents

[![Build][build-shield]][build-url]
[![Version][version-shield]][version-url]
[![Language][language-shield]][language-url]
[![Roadmap][roadmap-shield]][roadmap-url]
[![License][license-shield]][license-url]

\section about About The Project

This project started from a personal challenge: recreate **physical principles** in a **simulation** running in **real time**. I also wanted this project to easily **convey some understanding** of how it works, as well as **analysis** and **debugging tools**.

\note This project is currently in **active development**.<br>You can see a very messy **roadmap** [here](https://github.com/users/mlhoutel/projects/3).<br>If you want to **report a bug** or **request a feature**, see [here](https://github.com/mlhoutel/Simulatio/issues).

\subsection objectives Objectives

The objective was to learn more about such topics as:

-   **Gravity** (Classical mecanics one, he goal was not to model objects at the molecular level)
-   **Collisions** (With Newton's 3rd law)
-   **Friction** (Approximation with dissipation in heat)
-   **Electromagnetic fields** (_IN WORK_, see https://github.com/mlhoutel/Fields)
-   **Fluids** (_IN WORK_)

This project also allows me to familiarize myself with several areas of programming, such as:

-   **Graph Theory** and advanced **Data structures** like Quadtrees
-   **C++ programs/libraries** compilation (Mingw batch/ CMake)
-   **Memory allocation** and **Classes Inheritances**
-   **Optimisation** and **Algoritmics**
-   **Threads** and **Graphical acceleration** with CUDA (?)

<div style="margin: 50px; text-align:center">
<a href="https://github.com/mlhoutel/Simulatio/tree/main"><img style="width:80%" src="https://github.com/mlhoutel/Simulatio/blob/main/docs/screenshots/Pictures/Benchmarking.png"></a>
</div>

\subsection modularity Modularity

As this program is oriented towards testing and comparison, a modular structure has been set up.

**Modularity:**

-   **Precision** (at build time):
    -   [x] float
    -   [x] double
-   **Dimension** (at build time):
    -   [x] 2D
    -   [ ] 3D
-   **Broad phase structure** (at run time):
    -   [ ] Spacial grid
    -   [x] Quad-tree
    -   [ ] kD-tree
-   **Integration method** (at run time):
    -   [ ] Euler
    -   [x] Verlet
    -   [ ] RK4

\subsection summary Summary

<img align="left" height="100" style="padding:10px" src="./assets/icon/icon.png">

To summarize this is an introduction to the basic concepts and functions of 2D physical simulation (applicable to 3D thereafter).

The ultimate goal would be to present an easy-to-use and optimized physical engine, a bit like a simplified "Algodoo".

[build-shield]: https://img.shields.io/github/workflow/status/mlhoutel/Simulatio/Build/main?style=flat-square
[build-url]: https://github.com/mlhoutel/Simulatio/blob/main/.github/workflows/docs.yml
[version-shield]: https://img.shields.io/badge/version-0.5.2-orange?style=flat-square
[version-url]: https://github.com/mlhoutel/Simulatio/blob/main/CMakeLists.txt
[language-shield]: https://img.shields.io/github/languages/top/mlhoutel/Simulatio?style=flat-square
[language-url]: https://github.com/mlhoutel/Simulatio/search?l=cpp
[roadmap-shield]: https://img.shields.io/badge/roadmap-available-brightgreen?style=flat-square
[roadmap-url]: https://github.com/users/mlhoutel/projects/3
[license-shield]: https://img.shields.io/github/license/mlhoutel/Simulatio?style=flat-square
[license-url]: https://github.com/mlhoutel/Simulatio/blob/main/LICENSE/
