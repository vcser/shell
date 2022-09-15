CC = gcc
TARGET = shell
FILES := $(wildcard src/*.c)
HEADERS := $(wildcard src/*.h)

$(TARGET): $(FILES) $(HEADERS)
	-mkdir bin
	$(CC) $^ -o bin/$@