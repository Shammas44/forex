# créer un fichier compile_commands.json 
# make clean; bear -- make all

# FLAGS=-W -Wall -ansi -pedantic -std=c99 -g 
FLAGS=-g
FLAGS_TEST=-g -W -Wall -Wextra -Werror
OUT=app
BIN_DIR=bin/
INC_DIRS:=$(shell find . -type d -name "include")
INC_LIB= -I /usr/local/include
INC:=$(foreach dir,$(INC_DIRS),-I $(dir)) $(INC_LIB)

INC_LIB_DIR:=$(shell find lib -type d -name "include")
INC_STATIC_LIB:=$(foreach dir,$(INC_LIB_DIR),-I $(dir))

EVENTSLOOPLIB_SRC=-L lib/eventsLoop/bin -l eventsloop
MY_LIB=$(EVENTSLOOPLIB_SRC)

LIB=-l ssl -l crypto -l websockets $(MY_LIB) $(INC_STATIC_LIB)
LIB_TEST=-l criterion
INC_TEST=$(INC) -I tests 
CC=gcc
SRC_FILES = $(wildcard src/*.c)
TEST_FILES = $(wildcard tests/*.c)
SOURCES:=$(shell find ./src -type f -name "*.c" )
SOURCES_TEST=$(SOURCES) $(wildcard tests/*.c)
OBJS=$(SOURCES:%.c=%.o)
OBJS_TEST=$(OBJS) $(SOURCES_TEST:%.c=%.o)

.PHONY: all
all: lib main

.PHONY: lib
lib: 
	find ./lib/* -maxdepth 0 -type d -exec sh -c '(cd {} && make)' \;

.PHONY: bear 
bear:
	make clean; bear -- make test

main: $(OBJS) main.o
	$(CC) $(FLAGS) $^ -o $(BIN_DIR)$@ $(LIB) 

test: $(OBJS) $(TEST_FILES:.c=.o)
	$(CC) $(FLAGS_TEST) $(INC) $(INC_TEST) $(LIB) $(LIB_TEST) $^ -o $(BIN_DIR)$@

tests/%.o: tests/%.c
	$(CC) $(FLAGS_TEST) $(INC) $(INC_TEST) -c $< -o $@

%.o: %.c
	$(CC) $(FLAGS) $(INC) -c $< -o $@ -g

%: $(OBJS) %.o
	$(CC) $(FLAGS) $^ -o $@ && ./%

.PHONY: exec
exec: main
	./$(BIN_DIR)main

.PHONY: watch_main
watch_main: 
	watch -n 5 --color "make exec

# Remove every object file in the project
.PHONY: clean
clean: 
	@echo "cleaning..."
	find . -name "*.o" -delete; \
	find ./lib/* -maxdepth 0 -type d -exec sh -c '(cd {} && make clean)' \;

# Remove every exectuable file in the root directory
.PHONY: cleanx
cleanx:
	find ./$(BIN_DIR) -type f -perm +111 -maxdepth 1 -delete;

.PHONY: cleanall 
cleanall: clean cleanx

.PHONY: run_test
run_test: test
	./$(BIN_DIR)test

.PHONY: watch_test
watch_test: 
	watch -n 1 --color make run_test

.PHONY: core_enable core_disable

core_enable:
	ulimit -c unlimited

core_disable:
	ulimit -c 0
