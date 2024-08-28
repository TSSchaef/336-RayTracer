#Makefile

ray-tracer: main.o 
	gcc main.o -o ray-tracer 

main.o: src/main.c
	gcc src/main.c -c

#Second way to build the program with debugging enabled

#ray-tracer-debug: main-debug.o
#	gcc main-debug.o -o ray-tracer-debug

#main-debug.o: src/main.c
#	gcc -Wall -Werror -g src/main.c -c main-debug.o

.PHONY: clean

clean:
	rm -f *.o core* image.ppm ray-tracer *debug valgrind*
