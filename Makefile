#Makefile

ray-tracer: main.o 
	gcc main.o -o ray-tracer 

main.o: src/main.c
	gcc -Wall -Werror -g src/main.c -c

.PHONY: clean

clean:
	rm -f *.o core* ray-tracer valgrind*
