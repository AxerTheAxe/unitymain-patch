CC := x86_64-w64-mingw32-gcc
CFLAGS := -std=c99 -municode

SRC := src/patch.c
TARGET := build/patch.exe

$(TARGET): $(SRC) | build

build:
	mkdir -p build

$(TARGET): $(SRC)
	$(CC) $(CFLAGS) $(SRC) -o $(TARGET)

clean: 
	rm -rf build

.PHONY: all clean build
