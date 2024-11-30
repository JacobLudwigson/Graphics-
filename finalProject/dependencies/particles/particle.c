#include "particle.h"

std::vector <particle> redMatter;
glm::vec3 topBoundingVec;
glm::vec3 lowBoundingVec;
float boundingRadius;
float particleRadius;
float distanceThresholdToCombine;
float breakApartThreshold;
float randomVelocityThreshold;
float startingSize;

void setParamsParticles(glm::vec3 topBoundingVec_, glm::vec3 lowBoundingVec_,float boundingRadius_,
                        float particleRadius_, float distanceThresholdToCombine_, float breakApartThreshold_,
                        float randomVelocityThreshold_,float startingSize_){
    topBoundingVec = topBoundingVec_;
    lowBoundingVec = lowBoundingVec_;
    boundingRadius = boundingRadius_;
    particleRadius = particleRadius_;
    distanceThresholdToCombine = distanceThresholdToCombine_;
    breakApartThreshold = breakApartThreshold_;
    randomVelocityThreshold = randomVelocityThreshold_;
    startingSize = startingSize_;
}

void initParticles(){
    particle source;
    source.size = startingSize;
    source.position = glm::vec3(0,0,0);
    source.velocity = noiseVelocity() * 10.0f;
    // source.velocity = noiseVelocity()/100.0f;
    source.forwardVector = glm::normalize(source.position + source.velocity);
    redMatter.push_back(source);
}
void reverseVelocityYVector(particle *p){
    p->velocity.y = -p->velocity.y;
    p->forwardVector = glm::normalize(p->position + p->velocity);
}
void reverseVelocityXVector(particle *p){
    p->velocity.x = -p->velocity.x;
    p->forwardVector = glm::normalize(p->position + p->velocity);
}
void reverseVelocityZVector(particle *p){
    p->velocity.z = -p->velocity.z;
    p->forwardVector = glm::normalize(p->position + p->velocity);
}
void reverseXandZVectors(particle *p){
    // printf("Reversing! Pre reverse vector: [%f,%f,%f]\n", p->velocity.x,p->velocity.y, p->velocity.z);
    float temp = p->velocity.y;
    glm::vec3 normalVector = -p->position/sqrt(p->position * p->position);
    p->velocity = p->velocity - (2.0f * (p->velocity * normalVector)) * normalVector;
    p->velocity.y = temp;
    // p->velocity.z = p->velocity.z - (p->position.z/euclidDist);
    // p->velocity.x =  p->velocity.x - (p->position.x/euclidDist);
    // printf("Post reverse vector: [%f,%f,%f]\n", p->velocity.x,p->velocity.y, p->velocity.z);


    p->forwardVector = glm::normalize(p->position + p->velocity);
}
void drawParticles(GLuint shader, unsigned int redMatterTex, glm::vec3 pos){
    static std::random_device rd;
    static std::mt19937 gen(rd());
    std::uniform_real_distribution<> dis(0, 1.0); // Generate values between -0.5 and 0.5

    float lowBoundX = lowBoundingVec.x - boundingRadius;
    float topBoundingX = topBoundingVec.x + boundingRadius;
    float lowBoundY = lowBoundingVec.y;
    float topBoundingY = topBoundingVec.y;
    float lowBoundZ = lowBoundingVec.z - boundingRadius;
    float topBoundingZ = topBoundingVec.z + boundingRadius;
    float distX;
    float distY;
    float distZ;
    float breakApartChance;
    bool hitWall;
    // drawSphere(shader,pos, glm::vec3(10,10,10), 0, 0,redMatterTex);
    for (int i =0; i < redMatter.size(); i++){
        hitWall = false;
        // printf("Particle Size?: %f\n", redMatter[i].size);
        // printf("How many particles?: %d\n", redMatter.size());
        // printf("Particle %d Position [%f,%f,%f]\n", i, redMatter[i].position.x,redMatter[i].position.y,redMatter[i].position.z);
        // printf("Particle Velocity [%f,%f,%f]\n", redMatter[i].velocity.x,redMatter[i].velocity.y,redMatter[i].velocity.z);
        // printf("Particle Forward [%f,%f,%f]\n", redMatter[i].forwardVector.x,redMatter[i].forwardVector.y,redMatter[i].forwardVector.z);

        //Check if we need to bounce off the wall
        // if ((redMatter[i].position.x - (redMatter[i].size)) < lowBoundX || redMatter[i].position.x + (redMatter[i].size) > topBoundingX){
        //     hitWall = true;
        //     reverseVelocityXVector(&redMatter[i]);
        // }
        if ((redMatter[i].position.y - (redMatter[i].size)) <= lowBoundY || (redMatter[i].position.y + (redMatter[i].size)) >= topBoundingY){
            hitWall = true;
            // printf("Check 1: %f < %f\n",(redMatter[i].position.y - redMatter[i].size),lowBoundY);
            // printf("Check 2: %f < %f\n",(redMatter[i].position.y + redMatter[i].size),topBoundingY);
            // printf("Hitting Y wall, Size: %f, Y Pos : %f, Low BoundY : %f, Top BoundY : %f\n", redMatter[i].size, redMatter[i].position.y, lowBoundY, topBoundingY);
            // printf("Hitting Y wall", redMatter[i].size);

            reverseVelocityYVector(&redMatter[i]);
        }
        distX = redMatter[i].position.x - lowBoundingVec.x; 
        distZ = redMatter[i].position.z - lowBoundingVec.z;

        float distanceFromCenter = sqrt(distX * distX + distZ * distZ);
        
        if (distanceFromCenter + redMatter[i].size > boundingRadius) {
            hitWall = true;            
            // For X-axis bounce:
            reverseXandZVectors(&redMatter[i]);
           
        }
        if (hitWall && redMatter[i].size > particleRadius){
            breakApartChance = dis(gen);
            if (breakApartChance > breakApartThreshold){
                separateParticle(&redMatter[i]);
            }
        }        
        //Check if we need to combine particles
        for (int j =0; j < redMatter.size(); j++){
            if (j == i){
                continue;
            } 
            distX = fabs(fabs(redMatter[i].position.x - redMatter[j].position.x));
            distY = fabs(fabs(redMatter[i].position.y - redMatter[j].position.y));
            distZ = fabs(fabs(redMatter[i].position.z - redMatter[j].position.z));
            if ((distX - redMatter[i].size - redMatter[j].size) < distanceThresholdToCombine &&
                (distY - redMatter[i].size - redMatter[j].size) < distanceThresholdToCombine && 
                (distZ - redMatter[i].size - redMatter[j].size) < distanceThresholdToCombine){
                // combineParticles(&redMatter[i], &redMatter[j], j);
            }
        }
        redMatter[i].position = redMatter[i].position + (redMatter[i].velocity);
        // redMatter[i].velocity.y += dis(gen);
        // redMatter[i].velocity = glm::normalize(redMatter[i].velocity + noiseVelocity())/100.0f;
        redMatter[i].velocity = (redMatter[i].velocity);

        redMatter[i].forwardVector = glm::normalize(redMatter[i].position + redMatter[i].velocity);
        // redMatter[i].velocity = glm::normalize(redMatter[i].velocity + noiseVelocity());
        drawSphere(shader,pos + redMatter[i].position, glm::vec3(redMatter[i].size,redMatter[i].size,redMatter[i].size), 0, 0,redMatterTex);
    }
    return;
}
glm::vec3 noiseVelocity(){
    static std::random_device rd;
    static std::mt19937 gen(rd());
    std::uniform_real_distribution<> dis(-0.5, 0.5); // Generate values between -0.5 and 0.5
    float noiseVelocityX = dis(gen);
    float noiseVelocityY = (dis(gen));
    float noiseVelocityZ = dis(gen);

    // printf("Generated Velocity [%f,%f,%f]\n", noiseVelocityX,noiseVelocityY,noiseVelocityZ);


    return glm::vec3(noiseVelocityX/100,
                    noiseVelocityY/100,
                    noiseVelocityZ/100);
    //We could add some random noise to the velocity vectors depending on how they look.
}
void combineParticles(particle *p1, particle *p2, int p2Index){
    p1->size = pow(pow(p1->size,3) + pow(p2->size,3),0.3333);
    p1->velocity = (p1->velocity + p2->velocity)/2.0f;
    // p1->velocity = glm::normalize(glm::cross(p1->velocity, p2->velocity));
    // p1->position = (p2->position + p1->position)/2.0f;
    // p1->position = p1->position;

    redMatter.erase(redMatter.begin() + p2Index);
    return;
}
void separateParticle(particle *p1){
    particle p2;
    p1->size = pow(pow(p1->size,3)/2, 0.3333);

    //this could break some shit (Might go out of the box, we will see
    p2.size = p1->size;
    // p2.position = (p1->position + (p1->position + p1->forwardVector))/2.0f;
    p2.position = (p1->position - p1->forwardVector);
    // p2.position = (p1->position);


    p2.velocity = (p1->velocity/10.0f + noiseVelocity() * 10.0f);
    // p2.velocity.y += 0.01;
    redMatter.emplace_back(p2);
    return;
}