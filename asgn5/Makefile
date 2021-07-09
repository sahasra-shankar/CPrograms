CC = clang
CFLAGS = -Wall -Werror -Wextra -Wpedantic

all: sorting 

sorting: sorting.o counter.o set.o stack.o shell.o bubble.o quick.o heap.o
	$(CC) $(CFLAGS) -o sorting sorting.o counter.o set.o stack.o shell.o bubble.o quick.o heap.o 

sorting.o: sorting.c
	$(CC) $(CFLAGS) -c sorting.c

bubble.o: bubble.c
	$(CC) $(CFLAGS) -c bubble.c 

shell.o: shell.c
	$(CC) $(CFLAGS) -c shell.c 

heap.o: heap.c
	$(CC) $(CFLAGS) -c heap.c

quick.o: quick.c
	$(CC) $(CFLAGS) -c quick.c

set.o: set.c
	$(CC) $(CFLAGS) -c set.c

stack.o: stack.c
	$(CC) $(CFLAGS) -c stack.c

counter.o: counter.c
	$(CC) $(CFLAGS) -c counter.c

clean:
	rm -f sorting *.o

format: 
	clang-format -i -style=file *.c
