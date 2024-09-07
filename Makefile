#Makefile
BINARY=ray-tracer
CODEDIRS=src
INCDIRS=src ext 

CC=gcc
OPT=-O0
LIBFLAGS=-lm
DEPFLAGS=-MP -MD
CFLAGS=-Wall -Werror -g $(foreach D, $(INCDIRS), -I$(D)) $(OPT) $(DEPFLAGS)

CFILES=	$(foreach D,$(CODEDIRS), $(wildcard $(D)/*.c))
OBJECTS= $(patsubst %.c, %.o, $(CFILES)) 
DEPFILES= $(patsubst %.c, %.d, $(CFILES)) 

all: $(BINARY)


$(BINARY): $(OBJECTS)
	$(CC) -o $@ $^ $(LIBFLAGS)

%.o: %.c
	$(CC) $(CFLAGS) -c -o $@ $<

#TO-DO: Add second way to build the program with debugging disabled and optimiziations

.PHONY: clean

clean:
	rm -f $(OBJECTS) $(DEPFILES) $(BINARY) 

diff:
	$(info The status of the repository, and the volume of per-file changes:)
	@git status
	@git diff --stat

-include $(DEPFILES)
