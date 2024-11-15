# MinGW
ifeq ($(OS), Windows_NT)
CFLG=-O3 -Wall -I/mingw64/include
LIBS=-lglfw3 -lglew32 -lopengl32 -L/mingw64/lib
CLEAN=rm *.exe *.o *.a
else
# OSX
ifeq ($(shell uname), Darwin)
CFLG=-O3 -Wall -Wno-deprecated-declarations -DRES=1
LIBS=-framework GLUT -framework OpenGL
# Linux/Unix/Solaris
else
CFLG=-O3 -Wall
LIBS=-lglut -lGLU -lGL -lm
endif
CLEAN=rm -f gears *.o *.a
endif

CSCI_DIR = dependencies
CODE_DIR = code
SHAPES_DIR = dependencies/shapes
SHADEROPS_DIR = dependencies/shaderOps
SDL2 =

# Compile and link the final executable
final: main.o linked.a 
	gcc $(CFLG) -o final main.o linked.a $(LIBS)

# Compile .c files into .o object files
.c.o:
	gcc -c $(CFLG) -o $@ $<

# Object file generation rules
#Depending on how large our shapes h gets it may be worth combining these h's to depend on each other.
main.o: main.c $(CSCI_DIR)/linker.h $(SHAPES_DIR)/cube.h
	gcc -c $(CFLG) main.c -o main.o $(LIBS)

linked.a: cube.o shaderOps.o
	ar -rcs linked.a cube.o shaderOps.o

cube.o: $(SHAPES_DIR)/cube.c $(SHADEROPS_DIR)/shaderOps.h
	gcc -c $(CFLG) $(SHAPES_DIR)/cube.c -o cube.o $(LIBS)

shaderOps.o: $(SHADEROPS_DIR)/shaderOps.c $(CSCI_DIR)/headers.h
	gcc -c $(CFLG) $(SHADEROPS_DIR)/shaderOps.c -o shaderOps.o $(LIBS)






# Clean
clean:
	$(CLEAN)
