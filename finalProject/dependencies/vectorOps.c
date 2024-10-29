#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "vectorOps.h"
void setVec3(vec3 *v, float x, float y, float z) {
    v->x = x;
    v->y = y;
    v->z = z;
}
//This function will return v1 plus v2 as a vec3 object;
//Tested and good to go
vec3 addVec3(const vec3 v1, const vec3 v2) {
    vec3 temp;
    temp.x = v1.x + v2.x;
    temp.y = v1.y + v2.y;
    temp.z = v1.z + v2.z;
    return temp; 
}
//This function will return v1 MINUS v2 as a vec3 object;
//Tested and good to go
vec3 subTractVec3(vec3 v1, vec3 v2) {
    //v1 MINUS v2
    vec3 temp;
    temp.x = v1.x - v2.x;
    temp.y = v1.y - v2.y;
    temp.z = v1.z - v2.z;
    return temp;
}
vec3 crossOfTwoVectors(vec3 v1,vec3 v2){
   //v1 CROSS v2
   vec3 temp;
   temp.x = v1.y * v2.z - v1.z * v2.y;
   temp.y = v1.z * v2.x - v1.x * v2.z;
   temp.z = v1.x * v2.y - v1.y * v2.x;
   return temp;
}
float dot(vec3 v1, vec3 v2){
   return (v1.x*v2.x + v1.y*v2.y + v1.z*v2.z);
}
void normalizeVectorInPlace(vec3* v1){
    float length = sqrt(dot(*v1,*v1));
    v1->x = v1->x/length;
    v1->y = v1->y/length;
    v1->z = v1->z/length;
}
vec3 normalizeVector(vec3* v1){
    float length = sqrt(dot(*v1,*v1));
    vec3 temp;
    temp.x = v1->x/length;
    temp.y = v1->y/length;
    temp.z = v1->z/length;
    return temp;
}
void inPlaceVec3Negate(vec3 *v1){
   v1->x = -v1->x;
   v1->y = -v1->y;
   v1->z = -v1->z;
}
vec3 calculateSurfaceNormOfTriangle(vec3 v1, vec3 v2, vec3 v3){
   vec3 side1 = subTractVec3(v2,v1);
   vec3 side2 = subTractVec3(v3,v1);
   vec3 temp = crossOfTwoVectors(side1,side2);
   normalizeVectorInPlace(&temp);
   // inPlaceVec3Negate(&temp);
   return temp;
}
void absOfVector(vec3* v1){
   v1->x = fabs(v1->x);
   v1->y = fabs(v1->y);
   v1->z = fabs(v1->z);
}
