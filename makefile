SRC=src
OBJ=obj
BIN=bin
CFLAGS=-Wall -g -lcjson

SRCS=$(wildcard $(SRC)/*.c)
OBJS=$(patsubst $(SRC)/%.c, $(OBJ)/%.o, $(SRCS)) 

build: $(OBJS)
	gcc $^ ${CFLAGS} -o $(BIN)/program 

$(OBJ)/%.o: $(SRC)/%.c
	gcc ${CFLAGS} -c $^ -o $@ 

install: build
	sudo cp ./bin/program /usr/bin/miniweb

clean: 
	rm -f $(OBJ)/* $(BIN)/* 
