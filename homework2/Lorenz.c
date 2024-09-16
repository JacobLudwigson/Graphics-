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
#include <math.h>
#endif
//  Default resolution
//  For Retina displays compile with -DRES=2
#ifndef RES
#define RES 1
#endif

//  Globals
int th=0;       // Azimuth of view angle
int ph=0;       // Elevation of view angle
double z=0;     // Z variable
int mode = 0;
double dim=30;   // Dimension of orthogonal box

/*  Lorenz Parameters  */
double s  = 10;
double b  = 2.6666;
double r  = 28;



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

void getColor(double t, double maxT, float *red, float *green, float *blue)
{
    // Find the normalized time step we are currently on 
    // (This is our current dt/maxT (where maxT = (dt * 50000): This is dt on the 50000 iteration of the loop)
    double normalizedT = t / maxT;

    // Define gradient colors
    float startColor[3] = {0.0, 0.0, 10000.0}; // Red
    float endColor[3] = {1.0, 0.64, 0.0}; // Blue

    // calculate the current rgb values, but add normalized t * gradient difference. This gives us the amount we need to increase/decrease the gradient according to our current dt in the function.
    *red = startColor[0] + normalizedT * (endColor[0] - startColor[0]);
    *green = startColor[1] + normalizedT * (endColor[1] - startColor[1]);
    *blue = startColor[2] + normalizedT * (endColor[2] - startColor[2]);
}
/*
 *  Display the scene
 *  This function was taken and modified from ex6
 */
void display()
{
   //  Clear the image
   glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
   //  Reset previous transforms
   glLoadIdentity();
   //  Set view angle
   glRotated(ph,1,0,0);
   glRotated(th,0,1,0);

   //Enable Depth Testing
   glEnable(GL_DEPTH_TEST);
   glDepthFunc(GL_LESS);

   //  Draw axes in white
   glColor3f(1,1,1);
   //We need our axis to be large enough to see them with our current dim (which is enlarged so that the attrator points need not be normalized)
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
   //  Draw 10 pixel yellow points
   // glColor3f(1,1,0);
   // glPointSize(1);
   glBegin(GL_LINE_STRIP);
   int i;
   /*  Coordinates  */
   double x = 1;
   double y = 1;
   double zl = 1;
   double maxT = 50;
   /*  Time step  */
   double dt = 0.001;
   float red, green, blue = 0.0;
   //  Two dimensions
   for (i=0;i<50000;i++)
   {
      // printf("%d ", i);
      glColor3f(1,1,i%255);
      double dx = s*(y-x);
      double dy = x*(r-zl)-y;
      double dz = x*y - b*zl;
      x += dt*dx;
      y += dt*dy;
      zl += dt*dz;
      // we pass our red, green, and blue values (these are starting at 0)
      getColor(i*dt, maxT, &red, &green, &blue);
      //we color the current point in the line strip to be our calculated color in the gradient
      glColor3f(red, green, blue);
      glVertex3d(x,y,zl);
   }
   glEnd();

   //  Display parameters
   glColor3f(1,0,0);
   glWindowPos2i(5,5);
   Print("View Angle=%d,%d, Lorez Parameters: s = %f, b = %f, r = %f",th,ph, s, b, r);
   //  Flush and swap
   ErrCheck("display");
   
   glFlush();
   glutSwapBuffers();
}

/*
 *  GLUT calls this routine when a key is pressed
 */
void key(unsigned char ch,int x,int y)
{
   //  Exit on ESC
   if (ch == 27)
      exit(0);
   //  Reset view angle
   else if (ch == '0')
      th = ph = 0;
   // If the use wants to modify a lorenz parameter, they can click that character. We update the mode accordingly (s,b,r) => (1,2,3)
   else if (ch == 's'){
      mode = 1;
   }
   else if (ch == 'b'){
      mode = 2;
   }
   else if (ch == 'r'){
      mode = 3;
   }
   //  Increase the parameter the use wants to increase
   else if (ch == '+')
   {
      if (mode==1) s += 0.5;
      else if (mode==2) b += 0.5;
      else if (mode==3) r += 0.55;
   }
   //  Decrease the parameter the use wants to increase
   else if (ch == '-')
   {
      if (mode==1) s -= 0.5;
      else if (mode==2) b -= 0.5;
      else if (mode==3) r -= 0.5;
   }
   //  Tell GLUT it is necessary to redisplay the scene
   glutPostRedisplay();
}

/*
 *  GLUT calls this routine when an arrow key is pressed
 *  This function has been taken from ex6
 */ 
void special(int key,int x,int y)
{
   //  Right arrow key - increase azimuth by 5 degrees
   if (key == GLUT_KEY_RIGHT)
      th += 5;
   //  Left arrow key - decrease azimuth by 5 degrees
   else if (key == GLUT_KEY_LEFT)
      th -= 5;
   //  Up arrow key - increase elevation by 5 degrees
   else if (key == GLUT_KEY_UP)
      ph += 5;
   //  Down arrow key - decrease elevation by 5 degrees
   else if (key == GLUT_KEY_DOWN)
      ph -= 5;
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
   double asp = (height>0) ? (double)width/height : 1;
   // Instead of projecting into such a small box, allow the z values to be very large so that the user can really see how the function changes with parameter shifts 
   // If we project onto the X,Y of our screen, but leave the z relatively large we can avoid line clipping do to the projection onto small z values when the function has large values
   glOrtho(-asp*dim,+asp*dim, -dim,+dim, -1000.0,1000.0);
   //  Switch to manipulating the model matrix
   glMatrixMode(GL_MODELVIEW);
   //  Undo previous transformations
   glLoadIdentity();
}

/*
 *  Start up GLUT and tell it what to do
 *  This function has been taken from ex6 and slightly modified to add z buffering
 */
int main(int argc,char* argv[])
{
  //  Initialize GLUT and process user parameters
   glutInit(&argc,argv);

   //  Request double buffered, true color window 
   //  Since we have decided to make our image project to a large number of Z values, enable z buffering so we dont overlap
   glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
   //  Request 500 x 500 pixel window
   glutInitWindowSize(1920,1080);
   //  Create the window
   glutCreateWindow("Jacob Ludwigson Lorenz Attractor");
#ifdef USEGLEW
   //  Initialize GLEW
   if (glewInit()!=GLEW_OK) Fatal("Error initializing GLEW\n");
#endif
   //  Tell GLUT to call "display" when the scene should be drawn
   glutDisplayFunc(display);
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