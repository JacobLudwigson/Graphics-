/*
 *  Coordinates
 *
 *  Display 2, 3 and 4 dimensional coordinates in 3D.
 *
 *  Key bindings:
 *  1      2D coordinates
 *  2      2D coordinates with fixed Z value
 *  3      3D coordinates
 *  4      4D coordinates
 *  +/-    Increase/decrease z or w
 *  arrows Change view angle
 *  0      Reset view angle
 *  ESC    Exit
 */
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <math.h>
#include <time.h>
#include "vectorOps.h"
#include "../dependencies/CSCIx229.h"
#ifdef USEGLEW
#include <GL/glew.h>
#endif
//  OpenGL with prototypes for glext
#define GL_GLEXT_PROTOTYPES
#ifdef __APPLE__
#include <GLUT/glut.h>
// Tell Xcode IDE to not gripe about OpenGL deprecation
#pragma clang diagnostic ignored "-Wdeprecated-declarations"
#else
#include <GL/glut.h>
#endif
//  Default resolution
//  For Retina displays compile with -DRES=2
#ifndef RES
#define RES 1
#endif
//  Globals
float playerX = 0;
float playerY = 0;
float playerZ = 0;
int mode = 0;
int th=0;       // Azimuth of view angle
int ph=0;       // Elevation of view angle
double z=0;     // Z variable
double dim=30;   // Dimension of orthogonal box
double asp;
int positionsMatrix[50][7];
int displayMode =0;
float timestep = 0.0f;
float lightPosX = 1.0f;
float lightPosY = 10.0f;
float lightPosZ = 0.0f;
float angle = 0.0f;
const float radius = 30.0f;
float yOffsets[30];  // Array to store random y-offsets
const int Ni = 54;
vec3 norms[54];
float normsFloat[162];
float rgb[162];
vec3 viewerPos;
vec3 lightPos;
vec3 norms[54];

unsigned int texture[3];  //  Texture names



void populatePosMatrix(int seed){
    srand(123);
    int bound = 2*dim;
    for (int i = 0; i < 20; i++){
        positionsMatrix[i][0] = (rand()%bound)-dim;
        positionsMatrix[i][1] = (rand()%bound)-dim;
        positionsMatrix[i][2] = (rand()%bound)-dim;
        positionsMatrix[i][3] = (rand()%9) +1;
        positionsMatrix[i][4] = (rand()%9) +1;
        positionsMatrix[i][5] = (rand()%9) +1;
        positionsMatrix[i][6] = rand()%360;
    }
}

/*
 *  Display the scene
 *  This function was taken from from ex8
 */
static void cube(double x, double y, double z, 
                 double dx, double dy, double dz, 
                 double th,double ph,
                 float r, float g, float b, unsigned int texture_id) {
    //  Save transformation
    glPushMatrix();
    //  Offset
    glTranslated(x, y, z);
    glRotated(ph, 1, 0, 0);
    glRotated(th, 0, 1, 0);
    glScaled(dx, dy, dz);

    // Set color and bind texture
    glColor3f(r, g, b);
    glBindTexture(GL_TEXTURE_2D, texture_id);

    //  Enable texturing
   //  glEnable(GL_TEXTURE_2D);

    //  Cube
    glBegin(GL_QUADS);
    //  Front
    glNormal3f(0, 0, 1);
    glTexCoord2f(0, 0); glVertex3f(-1, -1, 1);
    glTexCoord2f(1, 0); glVertex3f(+1, -1, 1);
    glTexCoord2f(1, 1); glVertex3f(+1, +1, 1);
    glTexCoord2f(0, 1); glVertex3f(-1, +1, 1);
    //  Back
    glNormal3f(0, 0, -1);
    glTexCoord2f(0, 0); glVertex3f(+1, -1, -1);
    glTexCoord2f(1, 0); glVertex3f(-1, -1, -1);
    glTexCoord2f(1, 1); glVertex3f(-1, +1, -1);
    glTexCoord2f(0, 1); glVertex3f(+1, +1, -1);
    //  Right
    glNormal3f(1, 0, 0);
    glTexCoord2f(0, 0); glVertex3f(+1, -1, +1);
    glTexCoord2f(1, 0); glVertex3f(+1, -1, -1);
    glTexCoord2f(1, 1); glVertex3f(+1, +1, -1);
    glTexCoord2f(0, 1); glVertex3f(+1, +1, +1);
    //  Left
    glNormal3f(-1, 0, 0);
    glTexCoord2f(0, 0); glVertex3f(-1, -1, -1);
    glTexCoord2f(1, 0); glVertex3f(-1, -1, +1);
    glTexCoord2f(1, 1); glVertex3f(-1, +1, +1);
    glTexCoord2f(0, 1); glVertex3f(-1, +1, -1);
    //  Top
    glNormal3f(0, 1, 0);
    glTexCoord2f(0, 0); glVertex3f(-1, +1, +1);
    glTexCoord2f(1, 0); glVertex3f(+1, +1, +1);
    glTexCoord2f(1, 1); glVertex3f(+1, +1, -1);
    glTexCoord2f(0, 1); glVertex3f(-1, +1, -1);
    //  Bottom
    glNormal3f(0, -1, 0);
    glTexCoord2f(0, 0); glVertex3f(-1, -1, -1);
    glTexCoord2f(1, 0); glVertex3f(+1, -1, -1);
    glTexCoord2f(1, 1); glVertex3f(+1, -1, +1);
    glTexCoord2f(0, 1); glVertex3f(-1, -1, +1);
    //  End
    glEnd();

    //  Disable texturing
   //  glDisable(GL_TEXTURE_2D);

    //  Undo transformations
    glPopMatrix();
}

float xyzrgb[] = {
    // rocket cap 4 triangles
    1, 0, 0, 1, 0, 1,
    0.5, 1, 0, 1, 1, 0,
    0.5, 0, 1, 1, 0, 1,

    1, 0, 0, 1, 0, 1,
    0.5, 0, 1, 1, 0, 1,    
    0.5, -1, 0, 1, 1, 0,

    1, 0, 0, 1, 0, 1,
    0.5, 0, -1, 1, 0, 1,
    0.5, 1, 0, 1, 1, 0,


    1, 0, 0, 1, 0, 1,
    0.5, -1, 0, 1, 1, 0,
    0.5, 0, -1, 1, 0, 1,

    // rocket body 8 triangles
    -0.5, 0.5, 0.5, 1, 0, 1,
    0.5, 0, 1, 1, 0, 1,
    0.5, 1, 0, 1, 1, 0,


    -0.5, -0.5, 0.5, 1, 0, 1,
    0.5, -1, 0, 1, 1, 0,
    0.5, 0, 1, 1, 0, 1,

    -0.5, 0.5, -0.5, 1, 0, 1,
    0.5, 1, 0, 1, 1, 0,
    0.5, 0, -1, 1, 0, 1,

    -0.5, -0.5, -0.5, 1, 0, 1,
    0.5, 0, -1, 1, 0, 1,
    0.5, -1, 0, 1, 1, 0,

    0.5, 1, 0, 1, 0, 1,
    -0.5, 0.5, -0.5, 1, 0, 1,
    -0.5, 0.5, 0.5, 1, 1, 0,

    0.5, -1, 0, 1, 0, 1,
    -0.5, -0.5, 0.5, 1, 1, 0,
    -0.5, -0.5, -0.5, 1, 0, 1,

    -0.5, -0.5, -0.5, 1, 0, 1,
    -0.5, 0.5, -0.5, 1, 1, 0,
    0.5, 0, -1, 1, 0, 1,

    -0.5, -0.5, 0.5, 1, 0, 1,
    0.5, 0, 1, 1, 0, 1,
    -0.5, 0.5, 0.5, 1, 1, 0,


    // bottom fin 4 triangles
    -0.5, -0.5, 0.5, 1, 0, 1,
    -0.5, 0.5, 0.5, 1, 1, 0,
    -1, 0, 0, 1, 0, 1,

    -0.5, -0.5, -0.5, 1, 0, 1,
    -1, 0, 0, 1, 0, 1,
    -0.5, 0.5, -0.5, 1, 1, 0,

    -0.5, -0.5, -0.5, 1, 0, 1,
    -1, 0, 0, 1, 0, 1,
    -0.5, 0.5, 0.5, 1, 1, 0,

    -0.5, -0.5, 0.5, 1, 0, 1,
    -0.5, 0.5, -0.5, 1, 1, 0,
    -1, 0, 0, 1, 0, 1,
    
    // close the shape 2 triangles
    -0.5, 0.5, -0.5, 1, 0, 0,
    -0.5, 0, 0, 1, 1, 0,
    -0.5, 0.5, 0.5, 1, 1, 0,

    -0.5, -0.5, -0.5, 1, 0, 0,
    -0.5, -0.5, 0.5, 1, 1, 0,
    -0.5, 0, 0, 1, 1, 0,
};
float xyzrgbuv[] = {
    // rocket cap 4 triangles
    1, 0, 0, 1, 0, 1,  0, 0,
    0.5, 1, 0, 1, 1, 0,  0.5, 1,
    0.5, 0, 1, 1, 0, 1,  1, 0,

    1, 0, 0, 1, 0, 1,  0, 0,
    0.5, 0, 1, 1, 0, 1,  1, 0,
    0.5, -1, 0, 1, 1, 0,  0.5, 1,

    1, 0, 0, 1, 0, 1,  0, 0,
    0.5, 0, -1, 1, 0, 1,  1, 0,
    0.5, 1, 0, 1, 1, 0,  0.5, 1,

    1, 0, 0, 1, 0, 1,  0, 0,
    0.5, -1, 0, 1, 1, 0,  0.5, 1,
    0.5, 0, -1, 1, 0, 1,  1, 0,

    // rocket body 8 triangles
    -0.5, 0.5, 0.5, 1, 0, 1,  0, 0,
    0.5, 0, 1, 1, 0, 1,  1, 0,
    0.5, 1, 0, 1, 1, 0,  0.5, 1,

    -0.5, -0.5, 0.5, 1, 0, 1,  0, 0,
    0.5, -1, 0, 1, 1, 0,  0.5, 1,
    0.5, 0, 1, 1, 0, 1,  1, 0,

    -0.5, 0.5, -0.5, 1, 0, 1,  0, 0,
    0.5, 1, 0, 1, 1, 0,  0.5, 1,
    0.5, 0, -1, 1, 0, 1,  1, 0,

    -0.5, -0.5, -0.5, 1, 0, 1,  0, 0,
    0.5, 0, -1, 1, 0, 1,  1, 0,
    0.5, -1, 0, 1, 1, 0,  0.5, 1,

    0.5, 1, 0, 1, 0, 1,  0, 0,
    -0.5, 0.5, -0.5, 1, 0, 1,  1, 0,
    -0.5, 0.5, 0.5, 1, 1, 0,  0.5, 1,

    0.5, -1, 0, 1, 0, 1,  0, 0,
    -0.5, -0.5, 0.5, 1, 1, 0,  0.5, 1,
    -0.5, -0.5, -0.5, 1, 0, 1,  1, 0,

    -0.5, -0.5, -0.5, 1, 0, 1,  0, 0,
    -0.5, 0.5, -0.5, 1, 1, 0,  0.5, 1,
    0.5, 0, -1, 1, 0, 1,  1, 0,

    -0.5, -0.5, 0.5, 1, 0, 1,  0, 0,
    0.5, 0, 1, 1, 0, 1,  1, 0,
    -0.5, 0.5, 0.5, 1, 1, 0,  0.5, 1,

    // bottom fin 4 triangles
    -0.5, -0.5, 0.5, 1, 0, 1,  0, 0,
    -0.5, 0.5, 0.5, 1, 1, 0,  1, 0,
    -1, 0, 0, 1, 0, 1,  0.5, 1,

    -0.5, -0.5, -0.5, 1, 0, 1,  0, 0,
    -1, 0, 0, 1, 0, 1,  0.5, 1,
    -0.5, 0.5, -0.5, 1, 1, 0,  1, 0,

    -0.5, -0.5, -0.5, 1, 0, 1,  0, 0,
    -1, 0, 0, 1, 0, 1,  0.5, 1,
    -0.5, 0.5, 0.5, 1, 1, 0,  1, 0,

    -0.5, -0.5, 0.5, 1, 0, 1,  0, 0,
    -0.5, 0.5, -0.5, 1, 1, 0,  1, 0,
    -1, 0, 0, 1, 0, 1,  0.5, 1,

    // close the shape 2 triangles
    -0.5, 0.5, -0.5, 1, 0, 0,  0, 0,
    -0.5, 0, 0, 1, 1, 0,  1, 0,
    -0.5, 0.5, 0.5, 1, 1, 0,  0.5, 1,

    -0.5, -0.5, -0.5, 1, 0, 0,  0, 0,
    -0.5, -0.5, 0.5, 1, 1, 0,  1, 0,
    -0.5, 0, 0, 1, 1, 0,  0.5, 1,
};

void precomputeNormals(float* xyzrgb, vec3* norms){
   vec3 v1,v2,v3;
   int num = 324;
   int normCount = 0;
   for (int i = 0; i < num; i+=18){
      v1.x = xyzrgb[i];
      v1.y = xyzrgb[i+1];
      v1.z = xyzrgb[i+2];
      
      v2.x = xyzrgb[i+6];
      v2.y = xyzrgb[i+7];
      v2.z = xyzrgb[i+8];
      
      v3.x = xyzrgb[i+12];
      v3.y = xyzrgb[i+13];
      v3.z = xyzrgb[i+14];
      norms[normCount] = calculateSurfaceNormOfTriangle(v1,v2,v3);
      norms[normCount+1] = norms[normCount];
      norms[normCount+2] = norms[normCount];
      normCount += 3;
   }
}
void convertNormsToFloat(){
   for (int i = 0; i < 162; i+=3){
      int normIndex = i/3;
      normsFloat[i] = norms[normIndex].x;
      normsFloat[i+1] = norms[normIndex].y;
      normsFloat[i+2] = norms[normIndex].z;
   }
}

//if we initialize a new matrix for colors only, we can add in some fun colors if we want.
void shapeEasyWay(double x,double y,double z, double scalex,double scaley,double scalez, double th, unsigned int texture){
   // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
   // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
   // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
   // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

   glBindTexture(GL_TEXTURE_2D, texture);

   glVertexPointer(3,GL_FLOAT,8*sizeof(float),xyzrgbuv);
   glEnableClientState(GL_VERTEX_ARRAY);
   
   glNormalPointer(GL_FLOAT, 3 * sizeof(float),normsFloat);
   glEnableClientState(GL_NORMAL_ARRAY);

   glColorPointer(3,GL_FLOAT,8*sizeof(float),xyzrgbuv+3);
   glEnableClientState(GL_COLOR_ARRAY);

   glTexCoordPointer(2, GL_FLOAT, 8*sizeof(float), xyzrgbuv+6); 
   glEnableClientState(GL_TEXTURE_COORD_ARRAY);

   glPushMatrix();
   glTranslatef(x,y,z);
   glScalef(scalex,scaley,scalez);
   glRotatef(th,0,0,1);

   glDrawArrays(GL_TRIANGLES,0,Ni);
   glPopMatrix();
   glDisableClientState(GL_VERTEX_ARRAY);
   glDisableClientState(GL_COLOR_ARRAY);
   glDisableClientState(GL_NORMAL_ARRAY);
   glDisableClientState(GL_TEXTURE_COORD_ARRAY);
   glColor3f(1.0f, 0.0f, 0.0f); // Set color for normals (red)
   glBegin(GL_LINES);
   glEnd();
}


static void drawLight(){
   glColor3f(1,1,1);
   glPushMatrix();
   glEnable(GL_DEPTH_TEST);
   glDepthFunc(GL_LESS);
   glTranslatef(lightPosX, lightPosY, lightPosZ);
   // //GET RID OF THIS BEFORE SUBMISSION
   glutSolidSphere(5,10,10);
   glPopMatrix();
}
void getPointOnDownwardCurve(float* x, float* y, float* z, float dx, float dy, float dz, float scale, float rotation, float radius, float time){
   // *x = timestep;
   // *x = (*x * Cos(rotation))-(*z * Sin(rotation))+dx;
   *x = radius*Cos(rotation);
   *z = radius*Sin(rotation);
   *y = -scale* ((time-13)*(time-13))+dy;
   // *z = (*x * Sin(rotation))+(*z * Cos(rotation))+dz;
   // *z = 10*Sin(rotation);
   return;
}
void displayLightingGl(){
   glEnable(GL_LIGHTING);
   glEnable(GL_LIGHT0);

   GLfloat light_position[] = { lightPosX, lightPosY, lightPosZ, 1.0f };
   GLfloat max_light[] = { 1.0f, 1.0f, 1.0f, 1.0f };
   glLightfv(GL_LIGHT0, GL_POSITION, light_position);
   glLightfv(GL_LIGHT0, GL_AMBIENT, max_light);
   glLightfv(GL_LIGHT0, GL_DIFFUSE, max_light);
   glLightfv(GL_LIGHT0, GL_SPECULAR, max_light);
   // glLightfv(GL_LIGHT0, GL_S, light_diffuse);
   
   // GLfloat ambientLight[] = {0.2f, 0.2f, 0.2f, 1.0f}; // RGBA values
   // glLightfv(GL_LIGHT0, GL_AMBIENT, ambientLight);
   glEnable(GL_COLOR_MATERIAL);
   glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);
   GLfloat mat_ambient[] = { 0.5f, 0.5f, 0.5f, 1.0f };
   GLfloat mat_diffuse[] = { 0.5f, 0.5f, 0.5f, 1.0f };
   GLfloat mat_specular[] = { 1.0f, 1.0f, 1.0f, 1.0f };
   GLfloat mat_shininess[] = { 1.0f };
   glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, mat_ambient);
   glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, mat_diffuse);
   glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, mat_specular);
   glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, mat_shininess);
   glEnable(GL_TEXTURE_2D);
   float x = 0;
   float z = 0;
   float y = 0;
   int offsetIndex = 0;
   float randSizeVal = 0.0f;
   z = 0;
   // glTranslatef(1,0,1);
   for (int i = 0; i < 10; i++){
    //| cos(th)    0 -sin(th) | |   x   |= |XCos(th)- ZSin(th)|
    //|    0       1     0    | |   y   |  |        y         |
    //|  sin(th)   0  cos(th) | |   z   |  |XSin(th)+ ZCos(th)|
      // angle = i * 10;
      // glRotated(i,0,1,0);
      // int smallRandSize = rand()%5;
      offsetIndex+=1;
      randSizeVal = yOffsets[offsetIndex]+2.5;
      getPointOnDownwardCurve(&x,&y,&z,0,7+ yOffsets[offsetIndex],0,0.05, -10*i - 20, timestep, timestep);
      shapeEasyWay(x,y,z,2,randSizeVal,randSizeVal,270,texture[2]);
      // smallRandSize = rand()%5;
      offsetIndex+=1;
      randSizeVal = yOffsets[offsetIndex]+2.5;
      getPointOnDownwardCurve(&x,&y,&z,0,7+ yOffsets[offsetIndex],0,0.05, -18*i, timestep/2, timestep/2);
      shapeEasyWay(x,y,z,2,randSizeVal,5,180,texture[2]);
      // shapeEasyWay(-timestep,i,-timestep,2,5,2,270,texture[2]);
      // smallRandSize = rand()%5;
      offsetIndex+=1;
      randSizeVal = yOffsets[offsetIndex]+2.5;
      getPointOnDownwardCurve(&x,&y,&z,0,7+ yOffsets[offsetIndex],0,0.05, -16*i - 10, timestep/3, timestep/3);
      shapeEasyWay(x,y,z,5,2,randSizeVal,90,texture[2]);
      // timestep+=0.1;
      // getPointOnDownwardCurve(&x,&y,&z,0,7,0,0.05, -36*i, timestep-0.2);
      // shapeEasyWay(x,y,z,2,2,2,270,texture[2]);
   }
   shapeEasyWay(-timestep,timestep,-timestep,2,2,2,270,texture[2]);
   shapeEasyWay(timestep,timestep,-timestep,2,5,2,270,texture[2]);

   shapeEasyWay(-timestep/2,timestep,-timestep,5,1,1,270,texture[2]);
   shapeEasyWay(timestep/2,timestep,-timestep,2,3,2,270,texture[2]);

   shapeEasyWay(-timestep,timestep,-timestep/2,2,1,2,270,texture[2]);
   shapeEasyWay(timestep,timestep,-timestep/2,6,2,1,270,texture[2]);

   shapeEasyWay(-timestep/2,timestep/2,-timestep,2,1,6,270,texture[2]);
   shapeEasyWay(timestep,timestep/2,-timestep/2,7,1,2,270,texture[2]);
   for (int i = 0; i < 20; i++){
    //| cos(th)    0 -sin(th) | |   x   |= |XCos(th)- ZSin(th)|
    //|    0       1     0    | |   y   |  |        y         |
    //|  sin(th)   0  cos(th) | |   z   |  |XSin(th)+ ZCos(th)|
      // angle = i * 10;
      // glRotated(i,0,1,0);

   }

   shapeEasyWay(20,20,20,10,10,10,270,texture[2]);
   //contents of box
   cube(0,-9.75,0,19.75,10,9.75,0,0,0.5,0.5,0.5,texture[2]);
   //top of box
   cube(0,7,3.8,20,1,10,0,45,0.5,0.5,0.5,texture[0]);
   //actual box
   cube(0,-10,0,20,10,10,0,0,0.5,0.5,0.5,texture[0]);



   glDisable(GL_TEXTURE_2D);
   glDisable(GL_LIGHTING);
   drawLight();
}
void display()
{
   //  Clear the image
   glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
   //  Reset previous transforms
   glLoadIdentity();
   
   //  Set view angle
   glPushMatrix();
   glRotated(ph,1,0,0);
   glRotated(th,0,1,0);
   glTranslated(-playerX,-playerY,-playerZ);

   
   glEnable(GL_DEPTH_TEST);
   glDepthFunc(GL_LESS);




   //GET RID OF THIS GLUT OBJECT BEFORE SUBMISSION!!!! 
   // displayLightingNoGl();
   // displayLightingGl();
   displayLightingGl();



   //We need our axis to be large enough to see them with our current dim (which is enlarged so that the attrator points need not be normalized)
   //  Display parameters
   glPopMatrix();
   glColor3f(1,1,1);
   glWindowPos2i(5,5);
   //  Flush and swap
   ErrCheck("display");
   Print("View Angle: [ph,th] = [%d,%d]\n", ph,th);
   glFlush();
   glutSwapBuffers();
}
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
 *  GLUT calls this routine when a key is pressed
 *  This function has been taken and modified from ex6
 */
void key(unsigned char ch,int x,int y)
{
   //  Exit on ESC
   if (ch == 27)
      exit(0);
   //  Reset view angle
   else if (ch == '0')
      th = ph = 0;
   else if (ch == 'm'){
      mode+=1;
      mode %= 2;
      updateProjection();
   }
   if (ch == 'w'){
    updatePlayerCords(-0.2,th,ph); 
   }
   else if (ch == 'a'){
    updatePlayerCords(-0.2,th+90,180); 
   }
   else if (ch == 's'){
    updatePlayerCords(0.2,th,ph); 
   }
   //  Increase the parameter the use wants to increase
   else if (ch == 'd')
   {
    updatePlayerCords(-0.2,th-90,180); 
   }
   //  Tell GLUT it is necessary to redisplay the scene
   glutPostRedisplay();
}

/*
 *  GLUT calls this routine when an arrow key is pressed
 *  This function has been taken from ex6
 */ 

void special(int key,int x,int y)
{   //  Right arrow key - increase azimuth by 5 degrees
   if (key == GLUT_KEY_RIGHT) {
      th += 5;
      if (mode == 0){
         playerX = dim * -Sin(th) * Cos(ph);
         playerY = dim * Sin(ph);
         playerZ = dim * Cos(th) * Cos(ph);
      }
   }
   //  Left arrow key - decrease azimuth by 5 degrees
   else if (key == GLUT_KEY_LEFT) {
      th -= 5;
      if (mode == 0){
         playerX = dim * -Sin(th) * Cos(ph);
         playerY = dim * Sin(ph);
         playerZ = dim * Cos(th) * Cos(ph);
      }
   }
   //  Up arrow key - increase elevation by 5 degrees
   else if (key == GLUT_KEY_UP) {
      ph -= 5;
      if (mode == 0){
         playerX = dim * -Sin(th) * Cos(ph);
         playerY = dim * Sin(ph);
         playerZ = dim * Cos(th) * Cos(ph);
      }
   }
   //  Down arrow key - decrease elevation by 5 degrees
   else if (key == GLUT_KEY_DOWN) {
      ph += 5;
      if (mode == 0){
         playerX = dim * -Sin(th) * Cos(ph);
         playerY = dim * Sin(ph);
         playerZ = dim * Cos(th) * Cos(ph);
      }
   }
   //  Keep angles to +/-360 degrees
   th %= 360;
   ph %= 360;
   //  Tell GLUT it is necessary to redisplay the scene
   glutPostRedisplay();
}

/*
 *  GLUT calls this routine when the window is resized
 *  This function has been taken from ex6 and modified
 */
void reshape(int width,int height)
{
   //  Set the viewport to the entire window
   glViewport(0,0, RES*width,RES*height);
   //  Tell OpenGL we want to manipulate the projection matrix
   glMatrixMode(GL_PROJECTION);
   //  Undo previous transformations
   glLoadIdentity();
   //  Orthogonal projection box adjusted for the
   //  aspect ratio of the window
   asp = (height>0) ? (double)width/height : 1;
   // Instead of projecting into such a small box, allow the z values to be very large so that the user can really see how the function changes with parameter shifts 
   // If we project onto the X,Y of our screen, but leave the z relatively large we can avoid line clipping do to the projection onto small z values when the function has large values
   if (mode == 0){
      // glFrustum(-asp, asp, -1, 1, 1, 100);
   playerX= (dim)*Sin(th)*Cos(ph);
   playerY = (dim)*Sin(th)*Sin(ph);
   playerZ = (dim)*Cos(th);
    glOrtho(-asp*dim,+asp*dim, -dim,+dim, -dim*2,dim*2);
   }
   else if (mode == 1){
    glFrustum(-asp, asp, -1, 1, 1, 100);
   }

   //  Switch to manipulating the model matrix
   glMatrixMode(GL_MODELVIEW);
   //  Undo previous transformations
   glLoadIdentity();
}

void timer(int value) {
    angle += 0.02f; // Adjust the speed of the orbit
    lightPosX = radius * cos(angle);
    lightPosZ = radius * sin(angle);
    timestep += 0.2;
    if (timestep > 100){
      timestep =0;
    }
    glutPostRedisplay(); // Request a redraw
    glutTimerFunc(16, timer, 0); // Call timer function again after 16 ms (approx. 60 FPS)
}


// Function to initialize the random y-offsets
void initializeOffsets() {
    for (int i = 0; i < 30; i++) {
        yOffsets[i] = (rand() % 5) - 2.5;  // Random offset between -2.5 and +2.5
    }
}

/*
 *  Start up GLUT and tell it what to do
 *  This function has been taken from ex6 and slightly modified to add z buffering
 */
int main(int argc,char* argv[])
{
    //lets initialize some randomness!
    populatePosMatrix(0);
    initializeOffsets();
    // funColors();
    precomputeNormals(xyzrgb,norms);
    convertNormsToFloat();
  //  Initialize GLUT and process user parameters
   glutInit(&argc,argv);

   //  Request double buffered, true color window 
   //  Since we have decided to make our image project to a large number of Z values, enable z buffering so we dont overlap
   glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
   //  Request 500 x 500 pixel window
   glutInitWindowSize(1920,1080);
   //  Create the window
   glutCreateWindow("Jacob Ludwigson HW4 Pandoras Box");
#ifdef USEGLEW
   //  Initialize GLEW
   if (glewInit()!=GLEW_OK) Fatal("Error initializing GLEW\n");
#endif
   //  Tell GLUT to call "display" when the scene should be drawn
   glutDisplayFunc(display);
   glutTimerFunc(0, timer, 0);
   //  Tell GLUT to call "reshape" when the window is resized
   glutReshapeFunc(reshape);
   //  Tell GLUT to call "special" when an arrow key is pressed
   glutSpecialFunc(special);
   //  Tell GLUT to call "key" when a key is pressed
   glutKeyboardFunc(key);
   texture[0] = LoadTexBMP("textures/funkyTileTexture.bmp");
   texture[1] = LoadTexBMP("dependencies/crate.bmp");
   texture[2] = LoadTexBMP("textures/futuristicMosaic.bmp");
   //  Pass control to GLUT so it can interact with the user
   glutMainLoop();
   //  Return code
   return 0;
}