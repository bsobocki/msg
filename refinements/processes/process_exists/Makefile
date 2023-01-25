CC=g++
CFLAGS=-pedantic -Wall -g -O2
SRC_DIR=../../../src
INCLUDES=-I$(SRC_DIR)
SOURCES=$(wildcard $(SRC_DIR)/shared_memory/*.cpp)

compile:
	if [ ! -d bin ]; then mkdir bin; fi
	$(CC) $(CFLAGS) $(INCLUDES) $(SOURCES) proc.cpp -o bin/proc
	$(CC) $(CFLAGS) $(INCLUDES) $(SOURCES) proc2.cpp -o bin/proc2
