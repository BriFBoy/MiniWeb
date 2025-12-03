
SRC=./src/
OBJ = main.o content.o

build: $(OBJ)
	gcc main.o content.o -o program

main.o: $(SRC)main.c
	gcc -c $(SRC)main.c -o main.o

content.o: $(SRC)content.c
	gcc -c $(SRC)content.c -o content.o

clean: 
	rm -f content.o main.o program


