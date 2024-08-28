#Makefile

ray-tracer: main.o vector3.o ray.o 
	gcc main.o vector3.o ray.o -o ray-tracer -lm

main.o: src/main.c src/vector3.h
	gcc -Wall -Werror -g src/main.c -c

vector3.o: src/vector3.h src/vector3.c
	gcc -Wall -Werror -g src/vector3.c -c

ray.o: src/ray.h src/ray.c
	gcc -Wall -Werror -g src/ray.c -c

#Second way to build the program with debugging enabled

#ray-tracer-debug: main-debug.o
#	gcc main-debug.o -o ray-tracer-debug

#main-debug.o: src/main.c
#	gcc -Wall -Werror -g src/main.c -c main-debug.o

.PHONY: clean

clean:
	rm -f *.o core* image.ppm ray-tracer *debug valgrind*
