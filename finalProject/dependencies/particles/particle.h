#include "../shapes/halfCube.h"


struct particle{
    glm::vec3 position;
    glm::vec3 forwardVector;
    glm::vec3 velocity;
    float size;
};
extern std::vector <particle> redMatter;
extern glm::vec3 topBoundingVec;
extern glm::vec3 lowBoundingVec;
extern float boundingRadius;
extern float particleRadius;
extern float distanceThresholdToCombine;
extern float breakApartThreshold;
extern float randomVelocityThreshold;
extern float startingSize;

void setParamsParticles(glm::vec3 topBoundingVec_, glm::vec3 lowBoundingVec_,float boundingRadius_,
                        float particleRadius_, float distanceThresholdToCombine_, float breakApartThreshold_,
                        float randomVelocityThreshold_,float startingSize_);
void initParticles();
void reverseVelocityVector(particle *p);
void drawParticles(GLuint shader, unsigned int redMatterTex, glm::vec3 pos);
glm::vec3 noiseVelocity();
void reverseXandZVectors(particle *p);
void combineParticles(particle *p1, particle *p2, int p2Index);
void separateParticle(particle *p1);