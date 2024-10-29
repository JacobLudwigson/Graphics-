#  MinGW
ifeq "$(OS)" "Windows_NT"
CFLG=-O3 -Wall -DUSEGLEW -I/C/msys64/mingw64/include/SDL2
LIBS= -L/C/msys64/mingw64/lib -lSDL2 -lSDL2_mixer -lSDL2_image -lfreeglut -lglew32 -lglu32 -lopengl32
CLEAN=rm *.exe *.o *.a
else
#  OSX
ifeq "$(shell uname)" "Darwin"
CFLG=-O3 -Wall -Wno-deprecated-declarations -DRES=1
LIBS=-framework GLUT -framework OpenGL
#  Linux/Unix/Solaris
else
CFLG=-O3 -Wall
LIBS=-lglut -lGLU -lGL -lm
endif
#  OSX/Linux/Unix/Solaris
CLEAN=rm -f gears *.o *.a
endif
CSCI_DIR = dependencies
CODE_DIR = code
SDL2 = 
final: main.o vectorOps.o
	gcc $(CFLG) -o final main.o vectorOps.o $(LIBS)

.c.o:
	gcc -c $(CFLG) -o $@ $<
	
# Object file generation rules
main.o: main.c $(CSCI_DIR)/vectorOps.h $(CSCI_DIR)/sdlDocs.h
	gcc -c $(CFLG)  main.c -o main.o 

vectorOps.o: $(CSCI_DIR)/vectorOps.c 
	gcc -c $(CFLG) $(CSCI_DIR)/vectorOps.c -o vectorOps.o
sdlDocs.o: $(CSCI_DIR)/sdlDocs.c 
	gcc -c $(CFLG) $(CSCI_DIR)/sdlDocs.c -o sdlDocs.o

sdlDocs.o : sdlDocs.c 

# #  Compile and link 

#  Clean
clean:
	$(CLEAN)