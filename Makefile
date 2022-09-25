CC = g++
TARGET = shell
FILES := $(wildcard src/*.cpp)
HEADERS := $(wildcard src/*.h)

$(TARGET): $(FILES) $(HEADERS) | bin
	$(CC) $^ -o bin/$@

bin:
	mkdir bin

.PHONY: run

run: $(TARGET)
	./bin/$(TARGET)
