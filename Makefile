INC=includes
OUT=bin
SRC=src

build:
	gcc -o $(OUT)/main.o $(SRC)/main.c;

run:
	./$(OUT)/main.o

run-std:
	./$(OUT)/main.o --mode=std

run-child:
	./$(OUT)/main.o --mode=child
    
run-kill:
	./$(OUT)/main.o --mode=kill
    
run-posix:
	./$(OUT)/main.o --mode=posix

run-pipe:
	./$(OUT)/main.o --mode=pipe    
    
