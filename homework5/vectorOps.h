typedef struct {
    float x,y,z;
}vec3;

void setVec3(vec3 *v, float x, float y, float z);
vec3 addVec3(const vec3 v1, const vec3 v2);
vec3 subTractVec3(const vec3 v1, const vec3 v2);
vec3 crossOfTwoVectors(vec3 v1,vec3 v2);
float dot(vec3 v1, vec3 v2);
void normalizeVectorInPlace(vec3* v1);
vec3 normalizeVector(vec3* v1);
vec3 calculateSurfaceNormOfTriangle(vec3 v1, vec3 v2, vec3 v3);
void absOfVector(vec3* v1);
void inPlaceVec3Negate(vec3 *v1);
 