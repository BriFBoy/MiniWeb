SRC=src
OBJ=obj
BIN=bin

SRCS=$(wildcard $(SRC)/*.c)
OBJS=$(patsubst $(SRC)/%.c, $(OBJ)/%.o, $(SRCS)) 

build: $(OBJS)
	gcc $^ -g -o $(BIN)/program 

$(OBJ)/%.o: $(SRC)/%.c
	gcc -g -c $^ -o $@ 


clean: 
	rm -f $(OBJ)/* $(BIN)/* 
