CC = clang
CFLAGS = -Wall -Werror -Wextra -Wpedantic

all: banhammer

banhammer: banhammer.o speck.o parser.o node.o ll.o hash.o bv.o bf.o 
	$(CC) $(CFLAGS) -o banhammer banhammer.o parser.o speck.o node.o ll.o hash.o bv.o bf.o

speck.o: speck.c
	$(CC) $(CFLAGS) -c speck.c

parser.o: parser.c
	$(CC) $(CFLAGS) -c parser.c

bv.o: bv.c
	$(CC) $(CFLAGS) -c bv.c

hash.o: hash.c
	$(CC) $(CFLAGS) -c hash.c

ll.o: ll.c
	$(CC) $(CFLAGS) -c ll.c

node.o: node.c
	$(CC) $(CFLAGS) -c node.c

bf.o: bf.c
	$(CC) $(CFLAGS) -c bf.c

clean: 
	rm -f banhammer *.o

