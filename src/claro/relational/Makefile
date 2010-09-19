include ../../../hostdef/posix.mk

default: build
build: all

SOURCES = 		\
	relation.c 	\
	tuple.c

OBJECTS=${SOURCES:.c=.o}

CFLAGS+=-I../..

all: libclaro-relational.a libclaro-relational.so

libclaro-relational.a: $(OBJECTS)
	@printf "%5s    %-20s\n" LINK $@
	@ar cq $@ $(OBJECTS)

libclaro-relational.so: $(OBJECTS)
	@printf "%5s    %-20s\n" LINK $@
	@gcc $(CFLAGS) -shared -o $@ $(OBJECTS) -L../base -lclaro-base

.c.o:
	@printf "%5s    %-20s -> %-20s\n" CC $< $@
	@gcc $(CFLAGS) -c $< -o $@

clean:
	@for i in $(OBJECTS); do \
		rm -f $$i; \
		printf "%5s    %-20s\n" RM $$i; \
	done
	@rm -f *.a *.so
