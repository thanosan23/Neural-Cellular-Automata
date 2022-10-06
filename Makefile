CC=g++

CCFLAGS = -O2 -Wall
CCFLAGS += -Wno-missing-braces

INCFLAGS = -Iinclude/

LDFLAGS = -L lib/
LDFLAGS += -lraylib -lopengl32 -lgdi32 -lwinmm

BIN = ./bin
SRC=$(wildcard src/*.cpp) $(wildcard src/**/*.cpp)

.PHONY: all clean

all: dirs build

dirs:
	mkdir -p $(BIN)

build: dirs
	$(CC) $(SRC) -std=c++20 -o $(BIN)/main.exe $(CCFLAGS) $(INCFLAGS) $(LDFLAGS)

run: build
	$(BIN)/main.exe

clean:
	rm -rf $(BIN)
