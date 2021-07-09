CC = clang
CFLAGS = -Wall -Werror -Wextra -Wpedantic

all: gen dec err

gen: generator.o hamming.o bm.o
	$(CC) $(CFLAGS) -o generator generator.o hamming.o bm.o

generator.o: generator.c
	$(CC) $(CFLAGS) -c generator.c

dec: decoder.o hamming.o bm.o 
	$(CC) $(CFLAGS) -o decoder decoder.o hamming.o bm.o 

decoder.o: decoder.c
	$(CC) $(CFLAGS) -c decoder.c

err: error.o
	$(CC) $(CFLAGS) -o error error.o

error.o: error.c
	$(CC) $(CFLAGS) -c error.c

hamming.o: hamming.c
	$(CC) $(CFLAGS) -c hamming.c

bm.o: bm.c
	$(CC) $(CFLAGS) -c bm.c

clean: 
	rm -f generator *.o  decoder *.o

format:
	clang-format -i -style=file bm.c 
	clang-format -i -style=file hamming.c
	clang-format -i -style=file generator.c
	clang-format -i -style=file decoder.c
