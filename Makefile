# créer un fichier compile_commands.json 
# make clean; bear -- make all

# FLAGS=-W -Wall -ansi -pedantic -std=c99 -g 
FLAGS=-g
FLAGS_TEST=-W -Wall -Wextra -Werror
OUT=app
BIN_DIR=bin/
INC_DIRS:=$(shell find . -type d -name "include")
INC_LIB= -I /usr/local/include
INC:=$(foreach dir,$(INC_DIRS),-I $(dir)) $(INC_LIB)
LIB=-l ssl -l crypto -l websockets
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
all: main

.PHONY: bear 
bear:
	make clean; bear -- make all

main: $(OBJS) main.o
	$(CC) $(FLAGS) $^ -o $(BIN_DIR)$@ $(LIB) 

test: $(OBJS_TEST)
	$(CC) $(FLAGS_TEST) $(LIB_TEST) $^ -o $(BIN_DIR)$@

%.o: %.c
	$(CC) $(FLAGS) $(INC) -c $< -o $@ -g

%: $(OBJS) %.o
	$(CC) $(FLAGS) $^ -o $@ && ./%

.PHONY: exec
exec: main
	./$(BIN_DIR)main

.PHONY: watch_main
watch_main: 
	watch -n 5 --color "make exec | tail -n +2"

# Remove every object file in the project
.PHONY: clean
clean: 
	@echo "cleaning..."
	find . -name "*.o" -delete

# Remove every exectuable file in the root directory
.PHONY: cleanx
cleanx:
	find ./$(BIN_DIR) -type f -perm +111 -maxdepth 1 -delete   

.PHONY: cleanall 
cleanall: clean cleanx

.PHONY: run_test
run_test: test
	./$(BIN_DIR)test

.PHONY: watch_test
watch_test: 
	watch -n 1 --color make run_test
