SRC=src
OBJ=obj
BIN=bin
CFLAGS=-Wall -g -lcjson -IInclude -Werror
CFLAGS_RELEASE=-O3 -Wall -DNDEBUG -lcjson -IInclude

SRCS=$(wildcard $(SRC)/*.c) $(wildcard $(SRC)/*/*.c)
OBJS=$(patsubst $(SRC)/%.c, $(OBJ)/%.o, $(SRCS)) 
OBJDIRS := $(sort $(dir $(OBJS)))

build: $(OBJS)
	gcc $^ ${CFLAGS} -o $(BIN)/miniweb 

$(OBJDIRS): 
	mkdir -p $@

$(OBJ)/%.o: $(SRC)/%.c | $(OBJDIRS)
	gcc ${CFLAGS} -c $^ -o $@ 

release: clean
	$(MAKE) build CFLAGS="$(CFLAGS_RELEASE)"

install: release 
	sudo cp ./bin/miniweb /usr/bin/miniweb

clean: 
	rm -rf $(OBJ)/* $(BIN)/* 
