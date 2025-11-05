# Makefile for compiling all .cpp files in src directory
SRC_DIR := src
OBJ_DIR := build
BUILD_DIR := $(OBJ_DIR)/obj

CC = g++
CFLAGS = -O0 -g3 -Wall -c -fmessage-length=0
TARGET = SimpleView.exe

SRCS = $(wildcard $(SRC_DIR)/*.cpp)
OBJS = $(patsubst $(SRC_DIR)/%.cpp,$(BUILD_DIR)/%.o,$(SRCS))

all: $(TARGET)

$(TARGET): $(OBJ_DIR) $(BUILD_DIR) $(OBJS)
	$(CC) -o $(TARGET) $(OBJS) -lfreeglut -lopengl32 -lglu32

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp
	$(CC) $(CFLAGS) -o $@ $<

$(OBJ_DIR):
	mkdir $(OBJ_DIR)

$(BUILD_DIR):
	mkdir $(BUILD_DIR)

.PHONY: run
run: $(TARGET)
	./$(TARGET)

.PHONY: clean
clean:
	rm -f $(TARGET) $(OBJS)
	-rmdir $(BUILD_DIR) $(OBJ_DIR)
