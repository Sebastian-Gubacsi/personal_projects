# myopengl Project

This project demonstrates basic OpenGL rendering using C++ and the GLUT library. It is intended for educational purposes as part of CP411 Assignment 1.

## Features
- Simple OpenGL window
- Basic rendering example

## Prerequisites
- MinGW-w64 toolchain (g++, make)
- GLUT library (should be available in your environment)

## How to Build
1. Open a terminal in the `myopengl` directory.
2. Run:
   ```
   make
   ```
   This will compile `myopengl.cpp` and produce `myopengl.exe`.

## How to Run
After building, run:
```
make run
```
Or directly execute:
```
./myopengl.exe
```

## How to Clean
To remove the executable, run:
```
make clean
```

## Notes
- If you encounter issues with GLUT, ensure the DLLs are available in your PATH or in the project directory.
- This project is for Windows and uses MinGW-w64 tools.
