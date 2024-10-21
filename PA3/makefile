CC = g++
CFLAGS = -c
OPTFLAGS = -O3

all	: bin/cb
	@echo -n ""
bin/cb	: main.o
			$(CC) $(OPTFLAGS) main.o -Llib -o bin/cb
main.o 	   	: src/main.cpp
			$(CC) $(CFLAGS) $< -Ilib -o $@
clean:
		rm -rf *.o lib/*.a lib/*.o bin/*