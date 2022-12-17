PROJECT=communication
CC=g++
INCLUDES=
WRITER_SOURCES=writer.cpp
READER_SOURCES=reader.cpp
LIBS=
LDFLAGS= # -L$(LIBS)
CFLAGS=-pedantic -Wall -g -O2

$@:
	@if [ ! -d bin ]; then mkdir bin; fi
	@$(CC) $(CFLAGS) $(INCLUDE) $@.cpp $(LDFLAGS) -o bin/$@