## Mesh object model

### Mesh object model file format

numVerts numNorms numFaces
list of vertices, one vertex per line: 
x y z
list of normals, one normal per line: 
x y z
list of face data, one face per three lines:
number_of_vertices_of_face
list of vertex index of the face
list of normal index at each vertex

### Building, run, cleaning 


To build the project, open a terminal in the project root directory and run:

```sh
make
```

This will compile the source files in the `src/` directory and generate the executable meshview.exe. 

After building, run the executable:

```sh
make run
```

Clean object files and executable file. 

```sh
make clean
```

