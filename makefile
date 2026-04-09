SRC=src
OBJ=obj
BIN=bin
CFLAGS=-Wall -g -lcjson -IInclude -Werror

SRCS=$(wildcard $(SRC)/*.c) $(wildcard $(SRC)/*/*.c)
OBJS=$(patsubst $(SRC)/%.c, $(OBJ)/%.o, $(SRCS)) 
OBJDIRS := $(sort $(dir $(OBJS)))

build: $(OBJS)
	gcc $^ ${CFLAGS} -o $(BIN)/miniweb 

$(OBJDIRS): 
	mkdir -p $@

$(OBJ)/%.o: $(SRC)/%.c | $(OBJDIRS)
	gcc ${CFLAGS} -c $^ -o $@ 

install: build
	sudo cp ./bin/miniweb /usr/bin/miniweb

clean: 
	rm -rf $(OBJ)/* $(BIN)/* 
