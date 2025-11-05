# GLSL Demo Project

This project demonstrates the use of GLSL shaders in a C++ OpenGL application. It includes sample vertex and fragment shaders, and a modular codebase for learning and experimentation.

## Project Structure

- `src/` — Source code files (C++ classes and main application)
- `vertexshader.txt` — Example vertex shader
- `fragmentshader.txt` — Example fragment shader
- `makefile` — Build instructions

## Building the Project

To build the project, open a terminal in the project root directory and run:

```sh
make
```

This will compile the source files in the `src/` directory and generate the executable.

## Running the Project

After building, run the executable (the name may vary, e.g., `glsl_demo.exe`):

```sh
make run
```

Or double-click the executable in the file explorer. It shows a shining rotation teapot. That indicates shader programs vertexshader.txt and fragmentshader.txt are used. 

Stop the running program. Rename vertexshader.txt to vertexshader.glsl

 shader and fragement shader programs. 
Run the executable again. It shows the rotation teapot, but not as shining as previous running. That indicates the vertexshader is not loaded. Check that src/main.cpp, the following line is to load the vertex shader and fragement shader programs. 

``
shader = SM.loadfromFile("vertexshader.txt","fragmentshader.txt"); 
```

## Cleaning the Project

To remove compiled files and clean the workspace, run:

```sh
make clean
```

## More to Read

- **Source Code**: Start with `main.cpp` in the `src/` folder to understand the application entry point.
- **Shader Files**: Review `vertexshader.txt` and `fragmentshader.txt` for GLSL shader examples.
- **Class Files**: Explore the C++ classes in `src/` (`glApplication.*`, `glsl.*`, `glutWindow.*`, `glWindow.*`) for details on window management and shader integration.
- **Makefile**: Check the `makefile` for build instructions and dependencies.



