INC=includes
OUT=bin
SRC=src

build:
	gcc -o $(OUT)/main.o $(SRC)/main.c;

run:
	./$(OUT)/main.o 
    
