#Makefile

ray-tracer: main.o vector3.o ray.o sphere.o hittable.o
	gcc main.o vector3.o ray.o sphere.o hittable.o -o ray-tracer -lm

main.o: src/main.c src/vector3.h
	gcc -Wall -Werror -g src/main.c -c

vector3.o: src/vector3.h src/vector3.c
	gcc -Wall -Werror -g src/vector3.c -c

ray.o: src/ray.h src/ray.c
	gcc -Wall -Werror -g src/ray.c -c

hittable.o: src/hittable.h src/hittable.c
	gcc -Wall -Werror -g src/hittable.c -c

sphere.o: src/sphere.h src/sphere.c
	gcc -Wall -Werror -g src/sphere.c -c

#Second way to build the program with debugging enabled

.PHONY: clean

clean:
	rm -f *.o core* image.ppm ray-tracer *debug valgrind*
