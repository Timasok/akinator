I_FLAG := -I./include/ -I./lib/stack/include/ -I./lib/ #-I./inc/

CC:= g++
CFLAGS := $(I_FLAG) -g #-fsanitize=address  -Wall -fsanitize=address

SRC_DIR := ./src/
LIB_DIR := ./lib/
SRC_STK := ./lib/stack/src/

SOURCES 	:= $(SRC_DIR)main_ak.cpp $(SRC_DIR)akinator_f.cpp $(SRC_DIR)akinator_debug.cpp $(LIB_DIR)text_funcs.cpp $(SRC_STK)stack_funcs.cpp $(SRC_STK)debug_funcs.cpp 
EXECUTABLE  := akinator

all:
	mkdir -p graph_dumps 
	$(CC) $(SOURCES) $(CFLAGS) -o $(EXECUTABLE)