PROJECT=critical_section
CC=g++
INCLUDE=
SOURCES=shmem.cpp shmem_segment.cpp
LIBS=
LDFLAGS=-L$(LIBS)
CFLAGS=-pedantic -Wall -g -O2

build:
	@if [ ! -d bin ]; then mkdir bin; fi
	@$(CC) $(CFLAGS) $(INCLUDE) $(SOURCES) main.cpp -o bin/$(PROJECT)
	@$(CC) $(CFLAGS) $(INCLUDE) $(SOURCES) srv.cpp -o bin/$(PROJECT)_srv

run: build
	@./bin/$(PROJECT)

clean:
	@rm -rf ./bin/$(PROJECT) ./bin/$(PROJECT)_srv