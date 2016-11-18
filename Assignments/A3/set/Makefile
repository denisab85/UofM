CC=clang
CFLAGS=-I.
DEPS = table.h

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

table: table.o main.o
	$(CC) -o table table.o main.o -I.

clear:
	rm -f ./*.o ./table