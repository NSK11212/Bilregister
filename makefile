CC = gcc
CFLAGS = -Wall -Wextra -Werror 

SRC_DIR := src

Bilregister: main.o file.o
	$(CC) $(CFLAGS) main.o file.o -o Bilregister

main.o: $(SRC_DIR)/main.c $(SRC_DIR)/file.h
	$(CC) $(CFLAGS) -c $(SRC_DIR)/main.c -o main.o

file.o: $(SRC_DIR)/file.c $(SRC_DIR)/file.h
	$(CC) $(CFLAGS) -c $(SRC_DIR)/file.c -o file.o

clean:
	rm -f main.o file.o Bilregister

.PHONY: clean