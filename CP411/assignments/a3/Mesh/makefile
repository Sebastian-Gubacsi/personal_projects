# Makefile for compiling all .cpp files in src directory
SRC_DIR := src
BUILD_DIR := build

CC = g++
CFLAGS = -O0 -g3 -Wall -c -fmessage-length=0
TARGET = meshview.exe

SRCS = $(wildcard $(SRC_DIR)/*.cpp)
OBJS = $(patsubst $(SRC_DIR)/%.cpp,$(BUILD_DIR)/%.o,$(SRCS))

all: $(TARGET)

$(TARGET): $(BUILD_DIR) $(OBJS)
	$(CC) -o $(TARGET) $(OBJS) -lfreeglut -lopengl32 -lglu32

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp
	$(CC) $(CFLAGS) -o $@ $<

$(BUILD_DIR):
	mkdir $(BUILD_DIR)

.PHONY: run
run: $(TARGET)
	./$(TARGET)

.PHONY: clean
clean:
	rm -f $(TARGET) $(OBJS)
	rm -rf $(BUILD_DIR)
