CC = gcc
CFLAGS = -std=c99 -Wall -Wextra

main: main.o deque.o
	$(CC) $(CFLAGS) -o main main.o deque.o
main_o:
	$(CC) $(CFLAGS) -c main.c
deque_o:
	$(CC) $(CFLAGS) -c deque.c
clean:
	rm -f *.o main