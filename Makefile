PROJECT=msglib
CC=g++

CFLAGS=-pedantic -Wall -g -O2
GTEST_FLAGS=-lgtest -lgtest_main -lpthread

INCLUDE_SRC=-I src
INCLUDE_EXCEPTION=-I exception
INCLUDE_SHMEM=-I src/shared_memory
INCLUDE_SLOT_MSGQ=-I src/slotMsgQ
PROJ_INCLUDES=$(INCLUDE_SRC) $(INCLUDE_EXCEPTION) $(INCLUDE_SHMEM) $(INCLUDE_SLOT_MSGQ)

SHMEM_SRC=$(wildcard src/shared_memory/*.cpp)
SHARED_MEM_SRC=$(wildcard src/shared_memory/*.cpp)
SLOT_MSGQ_SRC=$(wildcard src/slotMsgQ/*/*.cpp)

build_project:
	@if [ ! -d bin ]; then mkdir bin; fi
	@$(CC) $(CFLAGS) $(PROJ_INCLUDES) $(SHMEM_SRC) -o bin/$(PROJECT)


UT_SRC=$(wildcard test/ut/*/*.cpp)
SHMEM_UT_INCLUDE=-I test/shared_memory_check
SHMEM_UT_SRC=$(wildcard test/shared_memory_check/*.cpp)
RUN_UT_SRC=test/ut/run_ut.cpp
UT_INCLUDES=$(INCLUDE_SRC) $(INCLUDE_EXCEPTION) $(INCLUDE_SLOT_MSGQ) $(SHMEM_UT_INCLUDE)

ut:
	@if [ ! -d bin/ut ]; then mkdir -p bin/ut; fi
	@$(CC) $(UT_INCLUDES) $(UT_SRC) $(RUN_UT_SRC) $(SHMEM_UT_SRC) $(SLOT_MSGQ_SRC)  $(CFLAGS) $(GTEST_FLAGS) -o bin/ut/tests
	@./bin/ut/tests

run:
	@./bin/$(PROJECT)

clean:
	@rm -rf bin