# Cube Project

This folder contains the source code for the Cube graphics assignment.

## Compile Instructions

1. Open PowerShell or Command Prompt.
2. Navigate to this folder:
   ```powershell
   cd path\to\cube
   ```
3. Compile the source code using MinGW:
   ```powershell
   gcc cube.c -o cube.exe -lglu32 -lopengl32 -lfreeglut
   ```  

## Run Instructions

- After compiling, run the executable:
  ```powershell
  .\cube.exe
  ```

## Clean Instructions

- To remove the executable and object files:
  ```powershell
  del cube.exe *.o
  ```

**Notes**
- Make sure all required OpenGL libraries are installed and available in your environment.
- If you encounter issues, check your compiler and library installation.
