#  MinGW
ifeq "$(OS)" "Windows_NT"
CFLG=-O3 -Wall -DUSEGLEW
LIBS=-lfreeglut -lglew32 -lglu32 -lopengl32
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
hw6: hw6.o vectorOps.o CSCIx229.a
	gcc $(CFLG) -o hw6 hw6.o vectorOps.o CSCIx229.a $(LIBS)
#  Create archive
CSCIx229.a: fatal.o errcheck.o print.o loadtexbmp.o loadobj.o projection.o
	ar -rcs CSCIx229.a fatal.o errcheck.o print.o loadtexbmp.o loadobj.o projection.o

.c.o:
	gcc -c $(CFLG) -o $@ $<

# Object file generation rules
fatal.o: $(CSCI_DIR)/fatal.c $(CSCI_DIR)/CSCIx229.h
	gcc -c $(CFLG) $(CSCI_DIR)/fatal.c -o fatal.o

errcheck.o: $(CSCI_DIR)/errcheck.c $(CSCI_DIR)/CSCIx229.h
	gcc -c $(CFLG) $(CSCI_DIR)/errcheck.c -o errcheck.o

print.o: $(CSCI_DIR)/print.c $(CSCI_DIR)/CSCIx229.h
	gcc -c $(CFLG) $(CSCI_DIR)/print.c -o print.o

loadtexbmp.o: $(CSCI_DIR)/loadtexbmp.c $(CSCI_DIR)/CSCIx229.h
	gcc -c $(CFLG) $(CSCI_DIR)/loadtexbmp.c -o loadtexbmp.o

loadobj.o: $(CSCI_DIR)/loadobj.c $(CSCI_DIR)/CSCIx229.h
	gcc -c $(CFLG) $(CSCI_DIR)/loadobj.c -o loadobj.o

projection.o: $(CSCI_DIR)/projection.c $(CSCI_DIR)/CSCIx229.h
	gcc -c $(CFLG) $(CSCI_DIR)/projection.c -o projection.o

vectorOps.o: $(CODE_DIR)/vectorOps.c $(CODE_DIR)/vectorOps.h
	gcc -c $(CFLG) $(CODE_DIR)/vectorOps.c -o vectorOps.o

hw6.o: $(CODE_DIR)/hw6.c $(CODE_DIR)/vectorOps.h $(CSCI_DIR)/CSCIx229.h
	gcc -c $(CFLG) $(CODE_DIR)/hw6.c -o hw6.o
# Create archive

# Linking the executable


# #  Compile and link 

#  Clean
clean:
	$(CLEAN)
