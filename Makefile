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
	./$(OUT)/main.o --mode=kill --signal=30 --pid=8888
    
run-posix:
	./$(OUT)/main.o --mode=posix --amount=5

run-pipe:
	./$(OUT)/main.o --mode=pipe    
    
