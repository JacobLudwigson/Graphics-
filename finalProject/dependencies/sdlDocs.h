#include <SDL/sdl.h>
#include <GL/gl.h>
#include <GL/glu.h>

#include <stdio.h>
#include <stdlib.h>

void display_bmp(char *file_name);
void putpixel(SDL_Surface *surface, int x, int y, Uint32 pixel);