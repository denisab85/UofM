CC=clang++
CFLAGS=-x c++
DEPS = ObjectManager.c, ObjectManager.h, Object.h

main:
	$(CC) $(CFLAGS) -o gc.bin main.c ObjectManager.c Object.h

clear:
	rm -f ./*.o ./gc.bin
