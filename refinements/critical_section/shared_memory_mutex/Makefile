PROJECT=critical_section
CC=g++
INCLUDE=
SOURCES=shmem.cpp shm_mutex.cpp main.cpp shmem_segment.cpp 
LIBS=
LDFLAGS=-L$(LIBS)
CFLAGS=-pedantic -Wall -g -O2

build:
	@if [ ! -d bin ]; then mkdir bin; fi
	@$(CC) $(CFLAGS) $(INCLUDE) $(SOURCES) -o bin/$(PROJECT)

run: compile
	@./bin/$(PROJECT)

clean:
	@rm -rf ./bin