CC = g++
CFLAGS = -c
OPTFLAGS = -O2

bin/mps	:  main_opt.o lib
			$(CC) $(OPTFLAGS)  main_opt.o -ltm_usage -Llib -o bin/mps
main_opt.o 	   	: src/main.cpp lib/tm_usage.h
			$(CC) $(CFLAGS) $< -Ilib -o $@