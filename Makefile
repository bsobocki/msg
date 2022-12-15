PROJECT=msglib
CC=g++
INCLUDE=
SOURCES=$(wildcard *.cpp)
LIBS=
LDFLAGS=-L$(LIBS)
CFLAGS=-pedantic -Wall -g -O2

build:
	@if [ ! -d bin ]; then mkdir bin; fi
	@$(CC) $(CFLAGS) $(INCLUDE) $(SOURCES) -o bin/$(PROJECT)

run:
	@./bin/$(PROJECT)

clean:
	@rm -rf bin