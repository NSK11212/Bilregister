CC = gcc
cflags = -Wall -Wextra -Werror -lto -jit

Bilregister: main.o file.o
	$(CC) $(cflags) main.o file.o -o Bilregister
main.o: main.c
	$(CC) $(cflags) -c src/main.c

file.o: file.c
	$(CC) $(cflags) -c src/file.c

clean:
	rm -f *.o Bilregister register.dat

.PHONY: clean