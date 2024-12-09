#include "../quickhull/quickHull.h"
class ParticleSystem {
    public:
        struct particle {
            glm::vec3 position;
            glm::vec3 forwardVector;
            glm::vec3 velocity;
            float size;
        };
    private:
        std::vector <particle> redMatter;
        glm::vec3 topBoundingVec;
        glm::vec3 lowBoundingVec;
        float boundingRadius;
        float particleRadius;
        float distanceThresholdToCombine;
        float breakApartThreshold;
        float combinationThreshold;
        float randomVelocityThreshold;
        float startingSize;
        float velocity = 1.0f;
        float drawSizeMult = 1.0f;
        int move = 1;
        bool drawParticle = true;
        bool qhull = false;
    public:
        void updateVelocity(float newV);
        void updateQhull(bool q);
        void updateMove(int move_);
        void updateDrawParticles(bool b);
        void updateDrawSizeMult(float m);
        void setParamsParticles(glm::vec3 topBoundingVec_, glm::vec3 lowBoundingVec_,float boundingRadius_,
                            float particleRadius_, float distanceThresholdToCombine_, float breakApartThreshold_,
                            float randomVelocityThreshold_,float startingSize_, float combinationThreshold_);
        void initParticles();
        void reverseVelocityYVector(particle *p);
        void reverseXandZVectors(particle *p);
        void populatePointsVector(QUICKHULL *qHull);
        void drawParticles(GLuint shader, unsigned int redMatterTex, glm::vec3 pos,QUICKHULL *qHull);
        void combineParticles(particle *p1, particle *p2, int p2Index);
        void separateParticle(particle *p1);
    private:
        glm::vec3 noiseVelocity();
};
