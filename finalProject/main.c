#define SDL_MAIN_HANDLED
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <math.h>
#include <SDL2/SDL.h>
#include <GL/glew.h>
#include <GL/gl.h>
#include <time.h>
#include "dependencies/vectorOps.h"
#ifdef USEGLEW
#include <GL/glew.h>
#endif
//  OpenGL with prototypes for glext
#define GL_GLEXT_PROTOTYPES
//  Default resolution
//  For Retina displays compile with -DRES=2
#ifndef RES
#define RES 1
#endif
#define Cos(x) (cos((x)*3.14159265/180))
#define Sin(x) (sin((x)*3.14159265/180))
// int main(int argc, char* argv[]){
//     SDL_Init(SDL_INIT_VIDEO);
//     SDL_Window* window = SDL_CreateWindow("More Lighting - SDL2",SDL_WINDOWPOS_UNDEFINED,SDL_WINDOWPOS_UNDEFINED,600,600,SDL_WINDOW_OPENGL|SDL_WINDOW_RESIZABLE);
//     // if (!window) Fatal("Cannot create window\n");
//     SDL_GL_CreateContext(window);
//     return 0;
// }

int axes=0;       //  Display axes
int mode=1;       //  Projection mode
int side=0;       //  Two sided mode
int ntex=1;       //  Texture mode
int th=0,ph=0;    //  View angles
int Th=0,Ph=30;   //  Light angles
float sco=180;    //  Spot cutoff angle
float Exp=0;      //  Spot exponent
int at0=100;      //  Constant  attenuation %
int at1=0;        //  Linear    attenuation %
int at2=0;        //  Quadratic attenuation %
int fov=53;       //  Field of view (for perspective)
int light=1;      //  Lighting
double asp=1;     //  Aspect ratio
double dim=8;     //  Size of world
float playerX,playerY,playerZ;
// Light values
int num       =   1;  // Number of quads
int inf       =   0;  // Infinite distance light
int smooth    =   1;  // Smooth/Flat shading
int local     =   0;  // Local Viewer Model
int emission  =   0;  // Emission intensity (%)
int ambient   =   0;  // Ambient intensity (%)
int diffuse   = 100;  // Diffuse intensity (%)
int specular  =   0;  // Specular intensity (%)
int shininess =   0;  // Shininess (power of two)
float X       = 0;    // Light X position
float Y       = 0;    // Light Y position
float Z       = 1;    // Light Z position

/*
 *  SDL calls this routine to display the scene
 */
void display(SDL_Window* window)
{
//    Wall(mode,side,ntex,axes,light,
//      th,ph,dim,Th,Ph,X,Y,Z,
//      emission,ambient,diffuse,specular,shininess,
//      sco,Exp,at0,at1,at2,
//      num,inf,smooth,local);
   SDL_GL_SwapWindow(window);
}

// /*
//  *  Call this routine when a key is pressed
//  *     Returns 1 to continue, 0 to exit
//  */
// int key(Mix_Chunk* boing)
// {
//    const Uint8* keys = SDL_GetKeyboardState(NULL);
//    int shift = SDL_GetModState()&KMOD_SHIFT;

//    //  Exit on ESC
//    if (keys[SDL_SCANCODE_ESCAPE])
//       return 0;
//    //  Reset view angle
//    else if (keys[SDL_SCANCODE_0])
//       X = Y = th = ph = 0;
//    //  Toggle axes
//    else if (keys[SDL_SCANCODE_B])
//       axes = 1-axes;
//    //  Toggle textures
//    else if (keys[SDL_SCANCODE_T])
//       ntex = 1-ntex;
//    //  Toggle lighting
//    else if (keys[SDL_SCANCODE_L])
//       light = 1-light;
//    //  Toggle infinity
//    else if (keys[SDL_SCANCODE_I])
//       inf = 1-inf;
//    //  Switch projection mode
//    else if (keys[SDL_SCANCODE_P])
//       mode = 1-mode;
//    //  Change number of quadrangles
//    else if ((keys[SDL_SCANCODE_KP_MINUS] || keys[SDL_SCANCODE_MINUS]) && num>1)
//       num--;
//    else if ((keys[SDL_SCANCODE_KP_PLUS] || keys[SDL_SCANCODE_EQUALS]) && num<100)
//       num++;
//    //  Increase/decrease spot azimuth
//    else if (keys[SDL_SCANCODE_LEFTBRACKET] && !shift)
//       Ph -= 5;
//    else if (keys[SDL_SCANCODE_RIGHTBRACKET] && !shift)
//       Ph += 5;
//    //  Increase/decrease spot cutoff angle
//    else if (keys[SDL_SCANCODE_LEFTBRACKET] && shift && sco>5)
//       sco = sco==180 ? 90 : sco-5;
//    else if (keys[SDL_SCANCODE_RIGHTBRACKET] && shift && sco<180)
//       sco = sco<90 ? sco+5 : 180;
//    //  Change spot exponent
//    else if (keys[SDL_SCANCODE_COMMA])
//    {
//       Exp -= 0.1;
//       if (Exp<0) Exp=0;
//    }
//    else if (keys[SDL_SCANCODE_PERIOD])
//       Exp += 0.1;
//    //  Change constant attenuation
//    else if (keys[SDL_SCANCODE_1] && !shift && at0>0)
//       at0--;
//    else if (keys[SDL_SCANCODE_1] && shift)
//       at0++;
//    //  Change linear attenuation
//    else if (keys[SDL_SCANCODE_2] && !shift && at1>0)
//       at1--;
//    else if (keys[SDL_SCANCODE_2] && shift)
//       at1++;
//    //  Change quadratic attenuation
//    else if (keys[SDL_SCANCODE_3] && !shift && at2>0)
//       at2--;
//    else if (keys[SDL_SCANCODE_3] && shift)
//       at2++;
//    //  Light position
//    else if (keys[SDL_SCANCODE_X] && !shift)
//       X -= 0.1;
//    else if (keys[SDL_SCANCODE_X] && shift)
//       X += 0.1;
//    else if (keys[SDL_SCANCODE_Y] && !shift)
//       Y -= 0.1;
//    else if (keys[SDL_SCANCODE_Y] && shift)
//       Y += 0.1;
//    else if (keys[SDL_SCANCODE_Z] && !shift)
//       Z -= 0.1;
//    else if (keys[SDL_SCANCODE_Z] && shift)
//       Z += 0.1;
//    //  Ambient level
//    else if (keys[SDL_SCANCODE_A] && !shift && ambient>0)
//       ambient -= 5;
//    else if (keys[SDL_SCANCODE_A] && shift && ambient<100)
//       ambient += 5;
//    //  Diffuse level
//    else if (keys[SDL_SCANCODE_D] && !shift && diffuse>0)
//       diffuse -= 5;
//    else if (keys[SDL_SCANCODE_D] && shift && diffuse<100)
//       diffuse += 5;
//    //  Specular level
//    else if (keys[SDL_SCANCODE_S] && !shift && specular>0)
//       specular -= 5;
//    else if (keys[SDL_SCANCODE_S] && shift && specular<100)
//       specular += 5;
//    //  Emission level
//    else if (keys[SDL_SCANCODE_E] && !shift && emission>0)
//       emission -= 5;
//    else if (keys[SDL_SCANCODE_E] && shift && emission<100)
//       emission += 5;
//    //  Shininess level
//    else if (keys[SDL_SCANCODE_N] && !shift && shininess>-1)
//       shininess -= 1;
//    else if (keys[SDL_SCANCODE_N] && shift && shininess<7)
//       shininess += 1;
//    //  Increase/decrease asimuth
//    else if (keys[SDL_SCANCODE_RIGHT])
//       th += 5;
//    else if (keys[SDL_SCANCODE_LEFT])
//       th -= 5;
//    //  Increase/decrease elevation
//    else if (keys[SDL_SCANCODE_UP])
//       ph += 5;
//    else if (keys[SDL_SCANCODE_DOWN])
//       ph -= 5;
//    //  PageUp key - increase dim
//    else if (keys[SDL_SCANCODE_PAGEDOWN])
//       dim += 0.1;
//    //  PageDown key - decrease dim
//    else if (keys[SDL_SCANCODE_PAGEUP] && dim>1)
//       dim -= 0.1;
//    //  Smooth color model
//    else if (keys[SDL_SCANCODE_F1])
//       smooth = 1-smooth;
//    //  Local Viewer
//    else if (keys[SDL_SCANCODE_F2])
//       local = 1-local;
//    //  Two sided mode
//    else if (keys[SDL_SCANCODE_F3])
//       side = 1-side;
//    //  Play boing once
//    else if (keys[SDL_SCANCODE_SPACE])
//       Mix_PlayChannel(-1,boing,0);

//    //  Wrap angles
//    Th %= 360;
//    Ph %= 360;
//    th %= 360;
//    ph %= 360;
//    //  Update projection
//    Project(mode?fov:0,asp,dim);
//    //  Return 1 to keep running
//    return 1;
// }
void updatePlayerCords(double stepSize, int tempTH, int tempPH){
    //if we take a Z unit vector and rotate it around the x axis counterclockwise we can calculate how our elevation angle will affect the direction of our movement: | 1   0         0   | |0| = |    0   |
    //                                                                                                                                                                | 0 cos(ph)  sin(ph)| |0|   |Sin(ph)|
    //                                                                                                                                                                | 0 -sin(ph) cos(ph)| |1|   |Cos(ph)|

    //Now we can take the resulting vector and counterclockwise rotate it around the Y axis, so as to calculate how our azimuth will change the direction of movement | cos(th)    0 -sin(th) | |   0   |= |-Sin(th) * Cos(ph)|
    //                                                                                                                                                                |    0       1     0    | |Sin(ph)|  |       Sin(ph)    |
    //                                                                                                                                                                |  sin(th)   0  cos(th) | |Cos(ph)|  | Cos(th) * Cos(ph)|
    float r2_x_1 = -Sin(tempTH) * Cos(tempPH);
    float r2_y_1 = Sin(tempPH);
    float r2_z_1 = Cos(tempTH) * Cos(tempPH);
    // Update player coordinates based on the direction vector and step size
    playerX += r2_x_1 * stepSize;
    playerY += r2_y_1 * stepSize;
    playerZ += r2_z_1 * stepSize;
}
void updateProjection(){
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    if (mode == 0){
      playerX = dim * -Sin(th) * Cos(ph);
      playerY = dim * Sin(ph);
      playerZ = dim * Cos(th) * Cos(ph);
      glOrtho(-asp*dim,+asp*dim, -dim,+dim, -dim*2,dim*2);
    }
    else if (mode == 1){
      glFrustum(-asp, asp, -1, 1, 1, 100);
      glMatrixMode(GL_MODELVIEW);
      glLoadIdentity();
      gluLookAt(playerX, playerY, playerZ,  // Eye position in the orthogonal view
                  0, 0, 0,  // Look at origin
                  0, 1, 0); // Up vector
      }
    glMatrixMode(GL_MODELVIEW);
}
/*
 *  Call this routine when the window is resized
 */
void reshape(SDL_Window* window)
{
   int width,height;
   SDL_GetWindowSize(window,&width,&height);
   //  Ratio of the width to the height of the window
   asp = (height>0) ? (double)width/height : 1;
   //  Set the viewport to the entire window
   glViewport(0,0, width,height);
   //  Set projection
//    Project(mode?fov:0,asp,dim);
}

/*
 *  Start up SDL and tell it what to do
 */
int main(int argc,char* argv[])
{
   int run=1;
   double t0=0;

   //  Initialize SDL
   SDL_Init(SDL_INIT_VIDEO);
   //  Set size, resizable and double buffering
   SDL_Window* window = SDL_CreateWindow("More Lighting - SDL2",SDL_WINDOWPOS_UNDEFINED,SDL_WINDOWPOS_UNDEFINED,600,600,SDL_WINDOW_OPENGL|SDL_WINDOW_RESIZABLE);
//    if (!window) Fatal("Cannot create window\n");
   SDL_GL_CreateContext(window);
#ifdef USEGLEW
   //  Initialize GLEW
   if (glewInit()!=GLEW_OK)
#endif
   //  Set screen size
   reshape(window);

   //  SDL event loop
//    ErrCheck("init");
   while (run)
   {
      //  Elapsed time in seconds
      double t = SDL_GetTicks()/1000.0;
      //  Process all pending events
      SDL_Event event;
      while (SDL_PollEvent(&event))
         switch (event.type)
         {
            case SDL_WINDOWEVENT:
               if (event.window.event==SDL_WINDOWEVENT_RESIZED)
               {
                  SDL_SetWindowSize(window,event.window.data1,event.window.data2);
                  reshape(window);
               }
               break;
            case SDL_QUIT:
               run = 0;
               break;
            case SDL_KEYDOWN:
            //    run = key(boing);
               break;
            default:
               //  Do nothing
               break;
         }
      //  Repeat key every 50 ms
      //  Display
      display(window);
      //  Slow down display rate to about 100 fps by sleeping 5ms
      SDL_Delay(5);
   }
   SDL_Quit();
   return 0;
}