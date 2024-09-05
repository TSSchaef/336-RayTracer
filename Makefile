#Makefile

ray-tracer: main.o vector3.o ray.o sphere.o material.o hittable_list.o camera.o aabb.o bvh.o texture.o image.o
	gcc main.o vector3.o ray.o sphere.o material.o hittable_list.o camera.o aabb.o bvh.o texture.o image.o -o ray-tracer -lm

main.o: src/main.c src/vector3.h
	gcc -Wall -Werror -g src/main.c -c

vector3.o: src/vector3.h src/vector3.c
	gcc -Wall -Werror -g src/vector3.c -c

ray.o: src/ray.h src/ray.c
	gcc -Wall -Werror -g src/ray.c -c

material.o: src/material.h src/material.c
	gcc -Wall -Werror -g src/material.c -c

sphere.o: src/sphere.h src/sphere.c
	gcc -Wall -Werror -g src/sphere.c -c

hittable_list.o: src/hittable_list.h src/hittable_list.c
	gcc -Wall -Werror -g src/hittable_list.c -c

camera.o: src/camera.h src/camera.c
	gcc -Wall -Werror -g src/camera.c -c

aabb.o: src/aabb.h src/aabb.c
	gcc -Wall -Werror -g src/aabb.c -c

bvh.o: src/bvh.h src/bvh.c
	gcc -Wall -Werror -g src/bvh.c -c

texture.o: src/texture.h src/texture.c
	gcc -Wall -Werror -g src/texture.c -c

image.o: src/image.h src/image.c
	gcc -Wall -Werror -g src/image.c -c

#Second way to build the program with debugging disabled

.PHONY: clean

clean:
	rm -f *.o core* image.ppm ray-tracer *debug valgrind*
