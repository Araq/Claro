CFLAGS= -I/sw/include/cairo -I/opt/local/include/cairo -I/usr/local/include/cairo -g
LDFLAGS=-L../../src/claro/base/ -lclaro-graphics -L../../src/claro/graphics/ -framework Cocoa -L/sw/lib -lcairo -lclaro-base -dynamic

all: test1

LIB_SRC=claro/core.d claro/base/*.d claro/graphics/*.d \
claro/graphics/widgets/*.d \
claro/graphics/cairo/*.d \
claro/graphics/cairooo/*.d

TEST1_SRC=test1.d $(LIB_SRC)
TEST1_OUT=test1
test1: $(TEST1_SRC)
	gdc $(TEST1_SRC) -o $(TEST1_OUT) $(LDFLAGS) $(CFLAGS)
