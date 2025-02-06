CC := x86_64-w64-mingw32-cc
CFLAGS := -std=c99 -pedantic -Wall -Wextra -Werror -municode

BUILD_DIR := build
SRC := src/patch.c
TARGET := $(BUILD_DIR)/patch.exe

all: $(TARGET)

$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

$(TARGET): $(BUILD_DIR) $(SRC)
	$(CC) $(CFLAGS) $(SRC) -o $(TARGET)

clean: 
	rm -rf $(BUILD_DIR)

.PHONY: all clean
