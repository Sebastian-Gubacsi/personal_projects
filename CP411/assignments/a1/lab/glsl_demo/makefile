# Define src directory
SRC_DIR := src

OBJ_DIR := build

# Define src directory

BUILD_DIR := $(OBJ_DIR)/obj


# Define compiler
CC = g++

# Define the C compiler and flags
CFLAGS = -O0 -g3 -Wall -c -fmessage-length=0

# Define the executable name
TARGET = glsl_demo.exe

# Define the object files
OBJS = $(BUILD_DIR)/glApplication.o $(BUILD_DIR)/glWindow.o $(BUILD_DIR)/glsl.o $(BUILD_DIR)/main.o $(BUILD_DIR)/glutWindow.o

# Default target: builds the executable
all: $(TARGET)  
$(TARGET):  $(OBJ_DIR) $(BUILD_DIR) $(OBJS) 
	$(CC) -o $(TARGET) $(OBJS) -lfreeglut -lglu32 -lopengl32 -lglew32 

# Rules for compiling individual object files
$(BUILD_DIR)/main.o: $(SRC_DIR)/main.cpp
	$(CC) $(CFLAGS) -o $(BUILD_DIR)/main.o $(SRC_DIR)/main.cpp

$(BUILD_DIR)/glApplication.o: $(SRC_DIR)/glApplication.cpp
	$(CC) $(CFLAGS) $(SRC_DIR)/glApplication.cpp -o $(BUILD_DIR)/glApplication.o

$(BUILD_DIR)/glWindow.o: $(SRC_DIR)/glWindow.cpp
	$(CC) $(CFLAGS) $(SRC_DIR)/glWindow.cpp -o $(BUILD_DIR)/glWindow.o

$(BUILD_DIR)/glsl.o: $(SRC_DIR)/glsl.cpp
	$(CC) $(CFLAGS) -o $(BUILD_DIR)/glsl.o $(SRC_DIR)/glsl.cpp
	
$(BUILD_DIR)/glutWindow.o:  $(SRC_DIR)/glutWindow.cpp
	$(CC) $(CFLAGS) -o $(BUILD_DIR)/glutWindow.o $(SRC_DIR)/glutWindow.cpp
	

$(OBJ_DIR):
	mkdir $(OBJ_DIR)

$(BUILD_DIR):	
	mkdir $(BUILD_DIR)
			
# Declare 'run' as a PHONY target
.PHONY: run
# Target to run the compiled program
run: $(TARGET)
	./$(TARGET)

# Phony target for cleaning up generated files
.PHONY: clean
clean:
	rm -f $(TARGET) $(OBJS)
	-rmdir $(BUILD_DIR) $(OBJ_DIR)
