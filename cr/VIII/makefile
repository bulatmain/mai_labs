CC = gcc
CFLAGS = -std=c99 -Wall -Wextra

main: main.o list.o
	$(CC) $(CFLAGS) -o main main.o list.o
main_o:
	$(CC) $(CFLAGS) -c main.c
deque_o:
	$(CC) $(CFLAGS) -c list.c
clean:
	rm -f *.o main
