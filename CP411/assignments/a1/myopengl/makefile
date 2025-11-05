# Makefile for myopengl project
# Usage:
#   make        # build the project
#   make run    # run the executable
#   make clean  # remove build artifacts

CC = g++
SRC = myopengl.cpp
EXE = myopengl.exe

all: $(EXE)

$(EXE): $(SRC)
	$(CC) $(SRC) -o $(EXE)

run: $(EXE)
	./$(EXE)

clean:
	del $(EXE)
