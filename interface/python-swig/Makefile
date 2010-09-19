CFLAGS=-I../../src -I/usr/include/python2.4 `pkg-config --cflags cairo gtk+-2.0`
#LDFLAGS=-lclaro-base -L../../src/claro/base/ -lpython2.4 -Wl,--export-dynamic -lclaro-graphics -L../../src/claro/graphics/ `pkg-config --libs cairo gtk+-2.0`

all: _claro.so

claro_wrap.c: claro.i
	swig -python claro.i

_claro.so: claro_wrap.o
	gcc claro_wrap.o -o _claro.so -shared $(LDFLAGS)

static: claro_wrap.o
	gcc claro_wrap.o ../../src/claro/graphics/libclaro-graphics.a ../../src/claro/base/libclaro-base.a -o _claro.so -shared -lpython2.4  `pkg-config --libs gtk+-2.0`	

clean:
	rm -rf *.c *.o *.so *.pyc claro.py
