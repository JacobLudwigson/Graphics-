#  MinGW
ifeq "$(OS)" "Windows_NT"
CFLG=-O3 -Wall -DUSEGLEW
LIBS=-lfreeglut -lglew32 -lglu32 -lopengl32
CLEAN=rm *.exe *.o *.a
else
#  OSX
ifeq "$(shell uname)" "Darwin"
CFLG=-O3 -Wall -Wno-deprecated-declarations -DRES=2
LIBS=-framework GLUT -framework OpenGL
#  Linux/Unix/Solaris
else
CFLG=-O3 -Wall
LIBS=-lglut -lGLU -lGL -lm
endif
#  OSX/Linux/Unix/Solaris
CLEAN=rm -f gears *.o *.a
endif

#  Compile and link
hw5: hw5.o vectorOps.o
	gcc $(CFLG) -o $@ $^ $(LIBS)

#  Compile source files into object files
hw5.o: hw5.c
	gcc $(CFLG) -c hw5.c

vectorOps.o: vectorOps.c
	gcc $(CFLG) -c vectorOps.c

#  Clean
clean:
	$(CLEAN)
