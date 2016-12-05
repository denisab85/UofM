CC=clang
CFLAGS=-I.
DEPS = set.h, set.o

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

main: main.o
	$(CC) -o main set.o main.o -I.

clear:
	rm -f ./*.o ./main