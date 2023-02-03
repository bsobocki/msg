PROJECT=msglib
CC=g++

CFLAGS=-pedantic -Wall -g -O2
GTEST_FLAGS=-lgtest -lgtest_main -lpthread

INCLUDE=-I src
INCLUDE_SLOT_MSGQ=-I src/slotMsgQ

SOURCES=$(wildcard src/*/*.cpp)
SHARED_MEM_SRC=$(wildcard src/shared_memory/*.cpp)
SLOT_MSGQ_SRC=$(wildcard src/slotMsgQ/*/*.cpp)

build_project:
	@if [ ! -d bin ]; then mkdir bin; fi
	@$(CC) $(CFLAGS) $(INCLUDE) $(SOURCES) -o bin/$(PROJECT)


UT_SRC=$(wildcard test/ut/*/*.cpp)
RUN_UT_SRC=test/ut/run_ut.cpp

ut:
	@if [ ! -d bin/ut ]; then mkdir -p bin/ut; fi
	@$(CC) $(INCLUDE) $(INCLUDE_SLOT_MSGQ) $(UT_SRC) $(RUN_UT_SRC) $(SHARED_MEM_SRC) $(SLOT_MSGQ_SRC)  $(CFLAGS) $(GTEST_FLAGS) -o bin/ut/tests
	@./bin/ut/tests

run:
	@./bin/$(PROJECT)

clean:
	@rm -rf bin