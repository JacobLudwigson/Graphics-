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

#define Cos(x) (cos((x)*3.14159265/180))
#define Sin(x) (sin((x)*3.14159265/180))
//  Globals
float playerX = 0;
float playerY = 0;
float playerZ = 0;
int mode = 0;
int th=0;       // Azimuth of view angle
int ph=260;       // Elevation of view angle
double z=0;     // Z variable
double dim=30;   // Dimension of orthogonal box
double asp;
int positionsMatrix[50][7];
int displayMode =0;

float lightPosX = 1.0f;
float lightPosZ = 0.0f;
float angle = 0.0f;
const float radius = 30.0f;

const int Ni = 54;
vec3 norms[54];
float normsFloat[162];
float rgb[162];
vec3 viewerPos;
vec3 lightPos;
vec3 norms[54];
typedef struct {
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float shininess;
}Material;

typedef struct {
    vec3 ColorAmbient;
    vec3 ColorDiffuse;
    vec3 ColorSpecular;
}Light;



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
 *  Convenience routine to output raster text
 *  Use VARARGS to make this more flexible
 */
#define LEN 8192  // Maximum length of text string

//This function was taken and modified from ex6
void Print(const char* format , ...)
{
   char    buf[LEN];
   char*   ch=buf;
   va_list args;
   //  Turn the parameters into a character string
   va_start(args,format);
   vsnprintf(buf,LEN,format,args);
   va_end(args);
   //  Display the characters one at a time at the current raster position
   while (*ch)
      glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18,*ch++);
}

/*
 *  Print message to stderr and exit
 *  This function was taken and modified from ex6
 */
void Fatal(const char* format , ...)
{
   va_list args;
   va_start(args,format);
   vfprintf(stderr,format,args);
   va_end(args);
   exit(1);
}

/*
 *  Check for OpenGL errors
 *  This function was taken and modified from ex6
 */
void ErrCheck(const char* where)
{
   int err = glGetError();
   if (err) fprintf(stderr,"ERROR: %s [%s]\n",gluErrorString(err),where);
}
/*
 *  Display the scene
 *  This function was taken from from ex8
 */
static void cube(double x,double y,double z,
                 double dx,double dy,double dz,
                 double th)
{
   //  Save transformation
   glPushMatrix();
   //  Offset
   glTranslated(x,y,z);
   glRotated(th,0,1,0);
   glScaled(dx,dy,dz);
   //  Cube
   glBegin(GL_QUADS);
   //  Front
   glColor3f(1,0,0);
   glVertex3f(-1,-1, 1);
   glVertex3f(+1,-1, 1);
   glVertex3f(+1,+1, 1);
   glVertex3f(-1,+1, 1);
   //  Back
   glColor3f(0,0,1);
   glVertex3f(+1,-1,-1);
   glVertex3f(-1,-1,-1);
   glVertex3f(-1,+1,-1);
   glVertex3f(+1,+1,-1);
   //  Right
   glColor3f(1,1,0);
   glVertex3f(+1,-1,+1);
   glVertex3f(+1,-1,-1);
   glVertex3f(+1,+1,-1);
   glVertex3f(+1,+1,+1);
   //  Left
   glColor3f(0,1,0);
   glVertex3f(-1,-1,-1);
   glVertex3f(-1,-1,+1);
   glVertex3f(-1,+1,+1);
   glVertex3f(-1,+1,-1);
   //  Top
   glColor3f(0,1,1);
   glVertex3f(-1,+1,+1);
   glVertex3f(+1,+1,+1);
   glVertex3f(+1,+1,-1);
   glVertex3f(-1,+1,-1);
   //  Bottom
   glColor3f(1,0,1);
   glVertex3f(-1,-1,-1);
   glVertex3f(+1,-1,-1);
   glVertex3f(+1,-1,+1);
   glVertex3f(-1,-1,+1);
   //  End
   glEnd();
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
void shapeEasyWay(double x,double y,double z, double scalex,double scaley,double scalez, double th){
   glVertexPointer(3,GL_FLOAT,6*sizeof(float),xyzrgb);
   glEnableClientState(GL_VERTEX_ARRAY);
   glNormalPointer(GL_FLOAT, 3 * sizeof(float),normsFloat);
   glEnableClientState(GL_NORMAL_ARRAY);
   //  Define colors for each vertex
    
    // glColorPointer(3,GL_FLOAT,6*sizeof(float),xyzrgb+3);
   glColorPointer(3,GL_FLOAT,6*sizeof(float),xyzrgb+3);
   glEnableClientState(GL_COLOR_ARRAY);
   glPushMatrix();
   glTranslatef(x,y,z);
   // glRotatef(th,0,0,1);
   glScalef(scalex,scaley,scalez);
   glDrawArrays(GL_TRIANGLES,0,Ni);
   glPopMatrix();
   glDisableClientState(GL_VERTEX_ARRAY);
   glDisableClientState(GL_COLOR_ARRAY);
   glColor3f(1.0f, 0.0f, 0.0f); // Set color for normals (red)
   glBegin(GL_LINES);
   glEnd();
}


static void drawLight(){
   glColor3f(1,1,1);
   glPushMatrix();
   glEnable(GL_DEPTH_TEST);
   glDepthFunc(GL_LESS);
   glTranslatef(lightPosX, 0, lightPosZ);
   // //GET RID OF THIS BEFORE SUBMISSION
   glutSolidSphere(5,10,10);
   glPopMatrix();
}

float lightCalculate(float M_e, float M_a, float M_d, float M_s,
                               float C_a, float C_d, float C_s,
                               vec3 surfaceNorm,int specularConst){
   
   //Emission Material = M_e, Ambient Material = M_a, Diffuse Material = M_d, Specular Material = M_s
   //Color Ambient = C_a, Color Diffuse: C_d, Color Specular: C_s
   //N = normal of surface, L is light position, H = half angle (L+V), S = specular constant
   //Color = ME +MACA + (N•L)MDCD + (N•H) SMSCS
   //
   vec3 normalizedLightPos = normalizeVector(&lightPos);
   vec3 normalizedViewerPos = normalizeVector(&viewerPos);
   vec3 temp = addVec3(normalizedLightPos,normalizedViewerPos);
   vec3 halfVector = normalizeVector(&temp);
   float N_L = fmax(dot(surfaceNorm,normalizedLightPos),0.0);
   float N_H = fmax(dot(surfaceNorm,halfVector),0.0);
   float ambient = M_a * C_a;
   float diffuse = N_L * M_d * C_d;
   float specular = pow(N_H, specularConst) * M_s * C_s;
   // printf("Calculated E_m, ambient,diffuse,specular : [%f,%f,%f,%f]\n",M_e,ambient,diffuse,specular);
   return M_e/2 + ambient + diffuse + specular;
}
void printRGBMatrix(){
   for (int i = 0; i < 54; i+=3){
      printf("RGB MATRIX: [%f,%f,%f]\n",rgb[i],rgb[i+1],rgb[i+2]);
   }
}

void updateColorValues(int numIndices, float* xyzrgb, 
                       vec3 M_a, vec3 M_d, vec3 M_s,
                       vec3 C_a, vec3 C_d, vec3 C_s,
                       int specularConst){
   vec3 currNorm;
   int normIndex;
   for (int i = 0; i < 324; i+=6){
      int val = floor(i/2);
      normIndex = floor(i/6);
      currNorm = norms[normIndex];
      // print("currNorm : [%f,%f,%f]",currNorm.x,currNorm.y,currNorm.z);      
      rgb[val] = lightCalculate(xyzrgb[i+3], M_a.x, M_d.x, M_s.x, C_a.x, C_d.x, C_s.x,currNorm,specularConst);
      rgb[val+1] = lightCalculate(xyzrgb[i+4], M_a.y, M_d.y, M_s.y, C_a.y, C_d.y, C_s.y,currNorm,specularConst);
      rgb[val+2] = lightCalculate(xyzrgb[i+5], M_a.z, M_d.z, M_s.z, C_a.z, C_d.z, C_s.z,currNorm,specularConst);
   }
}

void funShape(double x,double y,double z, double scalex,double scaley,double scalez, double th, Material mat, Light l1)
{
    //we can ditch our color values, although its probably not fast and instead update a random rgb matrix on every shape call
   glVertexPointer(3,GL_FLOAT,6*sizeof(float),xyzrgb);
   glEnableClientState(GL_VERTEX_ARRAY);
   //  Define colors for each vertex
   updateColorValues(Ni,xyzrgb,mat.ambient,mat.diffuse,mat.specular,l1.ColorAmbient,l1.ColorDiffuse,l1.ColorSpecular,mat.shininess);
    // glColorPointer(3,GL_FLOAT,6*sizeof(float),xyzrgb+3);
   glColorPointer(3,GL_FLOAT,3*sizeof(float),rgb);
   glEnableClientState(GL_COLOR_ARRAY);
   glPushMatrix();
   glTranslatef(x,y,z);
   // glRotatef(th,0,0,1);
   glScalef(scalex,scaley,scalez);
   glDrawArrays(GL_TRIANGLES,0,Ni);
   glPopMatrix();
   glDisableClientState(GL_VERTEX_ARRAY);
   glDisableClientState(GL_COLOR_ARRAY);
   glColor3f(1.0f, 0.0f, 0.0f); // Set color for normals (red)
   //  glBegin(GL_LINES);
   //  for (int i = 0; i < Ni; i++) {
   //      // Vertex position
   //      float vx = xyzrgb[i * 6]*scalex;
   //      float vy = xyzrgb[i * 6 + 1]*scaley;
   //      float vz = xyzrgb[i * 6 + 2]*scalez;
   //    //   printf("Vertices: [%f,%f,%f]\n",vx,vy,vz);
   //      // Normal vector

   //      int index = floor(i/9);
   //      float nx = norms[index].x;
   //      float ny = norms[index].y;
   //      float nz = norms[index].z;
   //    //   printf("Normal: [%f,%f,%f]\n",nx,ny,nz);
   //      // Draw line from vertex to vertex + normal
   //      glVertex3f(vx, vy, vz);
   //      glVertex3f((vx + nx)*2, (vy + ny)*2, (vz + nz)*2); // Scale normal for visibility
   //  }
   //  glEnd();
}
void displayLightingNoGl(){
   Light currLight = {
      .ColorAmbient = {0.0f, 0.0f, 0.0f},  // Low ambient reflection
      .ColorDiffuse = {1.0f, 1.0f, 1.0f},  // Moderate diffuse reflection
      .ColorSpecular = {1.0f, 1.0f, 1.0f}
   };

   Material metallicMaterial = {
        .ambient = {0.2f, 0.2f, 0.2f},  // Low ambient reflection
        .diffuse = {0.3f, 0.3f, 0.3f},  // Moderate diffuse reflection
        .specular = {0.8f, 0.8f, 0.8f}, // High specular reflection
        .shininess = 32.0f              // Higher shininess for a sharper specular highlight
    };

   funShape(0,0,0,10,10,10,0,metallicMaterial,currLight);
   // funShape(30,20,5,10,10,10,0,metallicMaterial,currLight);
   drawLight();
}
void displayLightingGl(){
   glEnable(GL_LIGHTING);
   glEnable(GL_LIGHT0);

   GLfloat light_position[] = { lightPosX, 0, lightPosZ, 1.0f };
   // GLfloat light_diffuse[] = { 1.0f, 1.0f, 1.0f, 1.0f };
   glLightfv(GL_LIGHT0, GL_POSITION, light_position);
   // glLightfv(GL_LIGHT0, GL_S, light_diffuse);
   
   // GLfloat ambientLight[] = {0.2f, 0.2f, 0.2f, 1.0f}; // RGBA values
   // glLightfv(GL_LIGHT0, GL_AMBIENT, ambientLight);
   glEnable(GL_COLOR_MATERIAL);
   glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);
   GLfloat mat_ambient[] = { 1.0f, 1.0f, 1.0f, 1.0f };
   GLfloat mat_diffuse[] = { 1.0f, 1.0f, 1.0f, 1.0f };
   GLfloat mat_specular[] = { 1.0f, 1.0f, 1.0f, 1.0f };
   GLfloat mat_shininess[] = { 1.0f };
   glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, mat_ambient);
   glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, mat_diffuse);
   glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, mat_specular);
   glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, mat_shininess);
   shapeEasyWay(0,0,0,10,10,10,1);
   // shapeEasyWay(20,20,20,10,10,10,1);
   glDisable(GL_LIGHTING);
   drawLight();
}
void display()
{
   setVec3(&viewerPos,playerX, playerY, playerZ);
   setVec3(&lightPos,lightPosX, 0.0f, lightPosZ);
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
   displayLightingNoGl();



   //We need our axis to be large enough to see them with our current dim (which is enlarged so that the attrator points need not be normalized)
   glColor3f(1,1,1);
   glBegin(GL_LINES);
   glVertex3d(0,0,0);
   glVertex3d(30,0,0);
   glVertex3d(0,0,0);
   glVertex3d(0,30,0);
   glVertex3d(0,0,0);
   glVertex3d(0,0,30);
   glEnd();
   //  Label axes
   glRasterPos3d(30,0,0);
   Print("X");
   glRasterPos3d(0,30,0);
   Print("Y");
   glRasterPos3d(0,0,30);
   Print("Z");
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
   else if (ch == '1'){

   }
   if (ch == 'w'){
    updatePlayerCords(-0.1,th,ph); 
   }
   else if (ch == 'a'){
    updatePlayerCords(-0.1,th+90,180); 
   }
   else if (ch == 's'){
    updatePlayerCords(0.1,th,ph); 
   }
   //  Increase the parameter the use wants to increase
   else if (ch == 'd')
   {
    updatePlayerCords(-0.1,th-90,180); 
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
   if (key == GLUT_KEY_RIGHT)
      th -= 5;
   //  Left arrow key - decrease azimuth by 5 degrees
   else if (key == GLUT_KEY_LEFT)
      th += 5;
   //  Up arrow key - increase elevation by 5 degrees
   else if (key == GLUT_KEY_UP)
      ph -= 5;
   //  Down arrow key - decrease elevation by 5 degrees
   else if (key == GLUT_KEY_DOWN)
      ph += 5;
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
    glOrtho(-asp*dim,+asp*dim, -dim,+dim, -dim*2,dim*2);
   }
   else if (mode == 1){
    glFrustum(-asp, asp, -1, 1, 1, 100);
   }
   else if (mode == 2){
      // playerX=30;
      // playerY=10;
    glFrustum(-asp, asp, -1, 1, 1, 100);
   }

   //  Switch to manipulating the model matrix
   glMatrixMode(GL_MODELVIEW);
   //  Undo previous transformations
   glLoadIdentity();
}

void timer(int value) {
    angle += 0.01f; // Adjust the speed of the orbit
    lightPosX = radius * cos(angle);
    lightPosZ = radius * sin(angle);

    glutPostRedisplay(); // Request a redraw
    glutTimerFunc(16, timer, 0); // Call timer function again after 16 ms (approx. 60 FPS)
}


/*
 *  Start up GLUT and tell it what to do
 *  This function has been taken from ex6 and slightly modified to add z buffering
 */
int main(int argc,char* argv[])
{
    //lets initialize some randomness!
    // populatePosMatrix(0);
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
   glutCreateWindow("Jacob Ludwigson 3d Scene with Perspectives");
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
   //  Pass control to GLUT so it can interact with the user
   glutMainLoop();
   //  Return code
   return 0;
}