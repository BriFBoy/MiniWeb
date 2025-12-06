SRC=src
OBJ=obj
BIN=bin

SRCS=$(wildcard $(SRC)/*.c)
OBJS=$(patsubst $(SRC)/%.c, $(OBJ)/%.o, $(SRCS)) 

build: $(OBJS)
	gcc $^ -o $(BIN)/program -g

$(OBJ)/%.o: $(SRC)/%.c
	gcc -c $^ -o $@ 


clean: 
	rm -f $(OBJ)/* $(BIN)/* 
