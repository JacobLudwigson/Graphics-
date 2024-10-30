# #  MinGW
# ifeq "$(OS)" "Windows_NT"
# CFLG=-O3 -Wall -lglfw3 -lglew32 -lopengl32 -o triangle -I/mingw64/include -L/mingw64/lib
# LIBS= 
# CLEAN=rm *.exe *.o *.a
# else
# #  OSX
# ifeq "$(shell uname)" "Darwin"
# CFLG=-O3 -Wall -Wno-deprecated-declarations -DRES=1
# LIBS=-framework GLUT -framework OpenGL
# #  Linux/Unix/Solaris
# else
# CFLG=-O3 -Wall
# LIBS=-lglut -lGLU -lGL -lm
# endif
# #  OSX/Linux/Unix/Solaris
# CLEAN=rm -f gears *.o *.a
# endif
# CSCI_DIR = dependencies
# CODE_DIR = code
# SDL2 = 
# final: main.o vectorOps.o
# 	gcc $(CFLG) -o final main.o vectorOps.o $(LIBS)

# .c.o:
# 	gcc -c $(CFLG) -o $@ $<
	
# # Object file generation rules
# main.o: main.c $(CSCI_DIR)/vectorOps.h $(CSCI_DIR)/sdlDocs.h
# 	gcc -c $(CFLG)  main.c -o main.o 

# vectorOps.o: $(CSCI_DIR)/vectorOps.c 
# 	gcc -c $(CFLG) $(CSCI_DIR)/vectorOps.c -o vectorOps.o
# sdlDocs.o: $(CSCI_DIR)/sdlDocs.c 
# 	gcc -c $(CFLG) $(CSCI_DIR)/sdlDocs.c -o sdlDocs.o

# sdlDocs.o : sdlDocs.c 

# # #  Compile and link 

# #  Clean
# clean:
# 	$(CLEAN)


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
SDL2 =

# Compile and link the final executable
final: main.o vectorOps.o
	gcc $(CFLG) -o final main.o vectorOps.o $(LIBS)

# Compile .c files into .o object files
.c.o:
	gcc -c $(CFLG) -o $@ $<

# Object file generation rules
main.o: main.c $(CSCI_DIR)/vectorOps.h 
	gcc -c $(CFLG) main.c -o main.o

vectorOps.o: $(CSCI_DIR)/vectorOps.c 
	gcc -c $(CFLG) $(CSCI_DIR)/vectorOps.c -o vectorOps.o


# Clean
clean:
	$(CLEAN)
