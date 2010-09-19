include ../../../hostdef/posix.mk
include widgets/widgets.mk

default: build
build: all

SOURCES = 		\
	block.c 	\
	claro.c 	\
	list.c		\
	log.c		\
	memory.c	\
	object.c	\
	object_map.c	\
	oscompat.c	\
	store.c

OBJECTS=${SOURCES:.c=.o}

CFLAGS+=-I../..

all: libclaro-base.a libclaro-base.so

libclaro-base.a: $(OBJECTS)
	@printf "%5s    %-20s\n" LINK $@
	@ar cq $@ $(OBJECTS)

libclaro-base.so: $(OBJECTS)
	@printf "%5s    %-20s\n" LINK $@
	@gcc $(CFLAGS) -shared -o $@ $(OBJECTS)

.c.o:
	@printf "%5s    %-20s -> %-20s\n" CC $< $@
	@gcc $(CFLAGS) -c $< -o $@

clean:
	@for i in $(OBJECTS); do \
		rm -f $$i; \
		printf "%5s    %-20s\n" RM $$i; \
	done
	@rm -f *.a *.so
