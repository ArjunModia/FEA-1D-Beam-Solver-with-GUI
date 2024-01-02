Finite Element Analysis (FEA) Solver
Overview

This repository contains the Finite Element Analysis (FEA) Solver implemented in C++, with a main focus on structural analysis. The main functionality is encapsulated in the main.cpp file, which serves as the entry point for the project.
Prerequisites

Before running the code, make sure the following dependencies are present in your system:

    Python 3.11.6
    Numpy 1.24.2
    Matplotlib 3.6.3
    fltk-1.3.9

Additionally, the following header files must be present in the same folder as the main.cpp file:

    matplotlibcpp.h
    postProcessing.h
    preProcessing.h
    userInputsGUI.h

Setup
Python Configuration

Ensure that Python 3.11.6 is installed on your system. If not, please download and install it from the official Python website.
fltk Configuration

Include the location of the Python 3.11 folder and the fltk syntax fltk-config --use-images --cxxflags --ldflags during compilation. The recommended compilation command for the tested system is as follows:

g++ main.cpp -o result `fltk-config --use-images --cxxflags --ldflags` -I/usr/include/python3.11 -lpython3.11

This command compiles the code and links the necessary libraries for Python and fltk.


Usage

After successful compilation, the output will be stored in a file named result. Run the compiled file using the following command:
./result

This command executes the FEA Solver, and the results will be displayed or saved as per the implementation.

Notes

    Ensure that all the required header files are in the same directory as the main.cpp file.
    The code has been tested for compatibility with Python 3.11.6, Numpy 1.24.2, Matplotlib 3.6.3, and fltk-1.3.9.
