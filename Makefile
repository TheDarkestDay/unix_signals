INC=includes
OUT=bin
SRC=src

$(OUT)/task1: $(OUT)/main.o $(OUT)/killmode.o $(OUT)/childmode.o $(OUT)/posixmode.o $(OUT)/pipemode.o $(OUT)/stdmode.o
	gcc -o $(OUT)/task1 $(OUT)/main.o $(OUT)/killmode.o $(OUT)/childmode.o $(OUT)/posixmode.o $(OUT)/pipemode.o $(OUT)/stdmode.o

$(OUT)/main.o: $(SRC)/main.c $(INC)/common.h
	gcc -I$(INC) -o $(OUT)/main.o -c $(SRC)/main.c

$(OUT)/stdmode.o: $(SRC)/stdmode.c $(INC)/common.h
	gcc -I$(INC) -o $(OUT)/stdmode.o -c $(SRC)/stdmode.c

$(OUT)/childmode.o: $(SRC)/childmode.c $(INC)/common.h
	gcc -I$(INC) -o $(OUT)/childmode.o -c $(SRC)/childmode.c

$(OUT)/posixmode.o: $(SRC)/posixmode.c $(INC)/common.h
	gcc -I$(INC) -o $(OUT)/posixmode.o -c $(SRC)/posixmode.c

$(OUT)/killmode.o: $(SRC)/killmode.c $(INC)/common.h
	gcc -I$(INC) -o $(OUT)/killmode.o -c $(SRC)/killmode.c

$(OUT)/pipemode.o: $(SRC)/pipemode.c $(INC)/common.h
	gcc -I$(INC) -o $(OUT)/pipemode.o -c $(SRC)/pipemode.c

clean:
	rm  $(OUT)/*;
