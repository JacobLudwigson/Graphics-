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

void displayLightingNoGl(){
   Light currLight = {
      .ColorAmbient = {0.5f, 0.5f, 0.5f},  // Low ambient reflection
      .ColorDiffuse = {0.3f, 0.3f, 0.3f},  // Moderate diffuse reflection
      .ColorSpecular = {0.8f, 0.8f, 0.8f}
   };

   Material metallicMaterial = {
        .ambient = {1.0f, 1.0f, 1.0f},  // Low ambient reflection
        .diffuse = {0.3f, 0.3f, 0.3f},  // Moderate diffuse reflection
        .specular = {0.8f, 0.8f, 0.8f}, // High specular reflection
        .shininess = 32.0f              // Higher shininess for a sharper specular highlight
    };

   funShape(0,0,0,10,10,10,0,metallicMaterial,currLight);
   drawLight();
}


void funShape(double x,double y,double z, double scalex,double scaley,double scalez, double th, Material mat, Light l1)
{
    //we can ditch our color values, although its probably not fast and instead update a random rgb matrix on every shape call
   glVertexPointer(3,GL_FLOAT,6*sizeof(float),xyzrgb);
   glEnableClientState(GL_VERTEX_ARRAY);
   //  Define colors for each vertex
   //Now we need to make some decisions about material values.
   updateColorValues(Ni,xyzrgb,mat.ambient,mat.diffuse,mat.specular,l1.ColorAmbient,l1.ColorDiffuse,l1.ColorSpecular,mat.shininess);
    
    // glColorPointer(3,GL_FLOAT,6*sizeof(float),xyzrgb+3);
   glColorPointer(3,GL_FLOAT,3*sizeof(float),xyzrgb+3);
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
    for (int i = 0; i < Ni; i++) {
        // Vertex position
        float vx = xyzrgb[i * 6]*scalex;
        float vy = xyzrgb[i * 6 + 1]*scaley;
        float vz = xyzrgb[i * 6 + 2]*scalez;
      //   printf("Vertices: [%f,%f,%f]\n",vx,vy,vz);
        // Normal vector

        int index = floor(i/9);
        float nx = norms[index].x;
        float ny = norms[index].y;
        float nz = norms[index].z;
      //   printf("Normal: [%f,%f,%f]\n",nx,ny,nz);
        // Draw line from vertex to vertex + normal
        glVertex3f(vx, vy, vz);
        glVertex3f((vx + nx)*2, (vy + ny)*2, (vz + nz)*2); // Scale normal for visibility
    }
    glEnd();
}


void getLightValues(GLenum light, Light* lightStruct) {
    GLfloat ambient[4], diffuse[4], specular[4];

    // Get the ambient color
    glGetLightfv(light, GL_AMBIENT, ambient);
    lightStruct->ColorAmbient.x = ambient[0];
    lightStruct->ColorAmbient.y = ambient[1];
    lightStruct->ColorAmbient.z = ambient[2];

    // Get the diffuse color
    glGetLightfv(light, GL_DIFFUSE, diffuse);
    lightStruct->ColorDiffuse.x = diffuse[0];
    lightStruct->ColorDiffuse.y = diffuse[1];
    lightStruct->ColorDiffuse.z = diffuse[2];

    // Get the specular color
    glGetLightfv(light, GL_SPECULAR, specular);
    lightStruct->ColorSpecular.x = specular[0];
    lightStruct->ColorSpecular.y = specular[1];
    lightStruct->ColorSpecular.z = specular[2];
}


void updateColorValues(int numIndices, float* xyzrgb, 
                       vec3 M_a, vec3 M_d, vec3 M_s,
                       vec3 C_a, vec3 C_d, vec3 C_s,
                       int specularConst){
   vec3 currNorm;
   int normIndex;
   for (int i = 0; i < 324; i+=6){
      //how the flibity skibity do I cast the xyz component of the triangles while iteratively calculating color values :|
      //We cant, must precompute normals!
      int val = floor(i/6);
      normIndex = floor(i/18);
      currNorm = norms[normIndex];
      
      rgb[val] = lightCalculate(xyzrgb[i+3], M_a.x, M_d.x, M_s.x, C_a.x, C_d.x, C_s.x,currNorm,specularConst);
      rgb[val+1] = lightCalculate(xyzrgb[i+4], M_a.y, M_d.y, M_s.y, C_a.y, C_d.y, C_s.y,currNorm,specularConst);
      rgb[val+2] = lightCalculate(xyzrgb[i+5], M_a.z, M_d.z, M_s.z, C_a.z, C_d.z, C_s.z,currNorm,specularConst);
      // printf("Calculated RGB : [%f,%f,%f]\n",rgb[val],rgb[val+1],rgb[val+2]);
   }
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
   return M_e + ambient + diffuse + specular;
}
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
