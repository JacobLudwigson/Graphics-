# MinGW
ifeq ($(OS), Windows_NT)
CFLG=-std=c++17 -Ofast -Wall -I/mingw64/include
LIBS= -DUSEGLEW -lglfw3 -lFreeImage -lglew32 -lglu32 -lopengl32  -L/mingw64/lib 
CLEAN=rm *.exe *.o *.a
else
# OSX
ifeq ($(shell uname), Darwin)
CFLG=-O3 -Wall -Wno-deprecated-declarations -DRES=1
#hopefully this works - apparently glfw relies on Cocoa and IOKit on Mac
LIBS=-lglfw -lfreeimage -lglew -framework OpenGL -framework Cocoa -framework IOKit -framework CoreVideo
# Linux/Unix/Solaris
else
CFLG=-std=c++17 -Ofast -Wall -I/usr/include -I/usr/local/include
LIBS= -lglfw -lfreeimage -lGLU -lGL -lm -L/usr/lib -L/usr/local/lib
endif
CLEAN=rm  *.o *.a
endif

CSCI_DIR = dependencies
CODE_DIR = code
SHAPES_DIR = dependencies/shapes
SHADEROPS_DIR = dependencies/shaderOps
MATERIAL_DIR = dependencies/materials
PARTICLE_DIR = dependencies/particles
QUICKHULL_DIR = dependencies/quickhull
LIGHT_DIR = dependencies/lights

# Compile and link the final executable
final: main.o linked.a
	g++ $(CFLG) -o final main.o linked.a $(LIBS)

# Compile .c files into .o object files
.c.o:
	gcc -c $(CFLG) -o $@ $<

# Object file generation rules
#Depending on how large our shapes h gets it may be worth combining these h's to depend on each other.
main.o: main.cpp $(CSCI_DIR)/linker.h 
	g++ -c $(CFLG) main.cpp -o main.o $(LIBS)

linked.a: cube.o shaderOps.o sphere.o cylinder.o material.o halfCube.o particle.o quickHull.o light.o
	ar -rcs linked.a cube.o shaderOps.o cylinder.o sphere.o material.o halfCube.o particle.o quickHull.o light.o

light.o: $(LIGHT_DIR)/light.c $(PARTICLE_DIR)/particle.h 
	g++ -c $(CLFG) $(LIGHT_DIR)/light.c -o light.o $(LIBS)

particle.o: $(PARTICLE_DIR)/particle.cpp $(QUICKHULL_DIR)/quickHull.h
	g++ -c $(CLFG) $(PARTICLE_DIR)/particle.cpp -o particle.o $(LIBS)

quickHull.o: $(QUICKHULL_DIR)/quickHull.cpp $(SHAPES_DIR)/halfCube.h 
	g++ -c $(CLFG) $(QUICKHULL_DIR)/quickHull.cpp -o quickHull.o $(LIBS)

halfCube.o: $(SHAPES_DIR)/halfCube.c $(SHAPES_DIR)/sphere.h 
	g++ -c $(CFLG) $(SHAPES_DIR)/halfCube.c -o halfCube.o $(LIBS)

sphere.o: $(SHAPES_DIR)/sphere.c $(SHAPES_DIR)/cylinder.h
	g++ -c $(CFLG) $(SHAPES_DIR)/sphere.c -o sphere.o $(LIBS)

cylinder.o: $(SHAPES_DIR)/cylinder.c $(SHAPES_DIR)/cube.h
	g++ -c $(CFLG) $(SHAPES_DIR)/cylinder.c -o cylinder.o $(LIBS)

cube.o: $(SHAPES_DIR)/cube.c $(SHADEROPS_DIR)/shaderOps.h
	g++ -c $(CFLG) $(SHAPES_DIR)/cube.c -o cube.o $(LIBS)

shaderOps.o: $(SHADEROPS_DIR)/shaderOps.cpp $(MATERIAL_DIR)/material.h
	g++ -c $(CFLG) $(SHADEROPS_DIR)/shaderOps.cpp -o shaderOps.o $(LIBS)

material.o: $(MATERIAL_DIR)/material.cpp $(CSCI_DIR)/headers.h
	g++ -c $(CFLG) $(MATERIAL_DIR)/material.cpp -o material.o $(LIBS)





# Clean
clean:
	$(CLEAN)
