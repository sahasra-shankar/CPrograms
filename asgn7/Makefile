CC = clang
CFLAGS = -Wall -Werror -Wextra -Wpedantic

all: encode decode

encode: encode.o io.o trie.o 
	$(CC) $(CFLAGS) -o encode encode.o io.o trie.o -lm 

decode: decode.o io.o trie.o word.o
	$(CC) $(CFLAGS) -o decode decode.o io.o trie.o word.o -lm

encode.o: encode.c
	$(CC) $(CFLAGS) -c encode.c

decode.o: decode.c
	$(CC) $(CFLAGS) -c decode.c

io.o: io.c
	$(CC) $(CFLAGS) -c io.c

trie.o: trie.c
	$(CC) $(CFLAGS) -c trie.c

word.o: word.c
	$(CC) $(CFLAGS) -c word.c

clean:
	rm -f encode decode *.o

format:
	clang-format -i -style=file *.c *.h
