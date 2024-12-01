#include "particle.h"

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
// glm::vec3 slerpInterpolation(glm::vec3 x1, glm::vec3 x2){
//     sp = x1 - cp 
//     ep = x2 - cp 
//     // compute the lengths of these vectors
//     lsp = sqrt(sum(sp^2))
//     lep = sqrt(sum(ep^2))
//     // scale the vectors
//     s1 = lep * sp
//     s2 = lsp * ep
//     // compute the common length of s1 and s2
//     l = sqrt(sum(s1^2))
//     // normalize s1 and s2
//     s1 = s1 / l
//     s2 = s2 / l
//     // compute angle between s1 and s2
//     phi = acos(sum(s1 * s2)/(sqrt(sum(s1^2)) * sqrt(sum(s2^2))))
//     // slerp
//     slerp = cp + R * (sin((1-t)*phi)/sin(phi) * s1 + sin(t*phi)/sin(phi) * s2)
// }
void setParamsParticles(glm::vec3 topBoundingVec_, glm::vec3 lowBoundingVec_,float boundingRadius_,
                        float particleRadius_, float distanceThresholdToCombine_, float breakApartThreshold_,
                        float randomVelocityThreshold_,float startingSize_, float combinationThreshold_){
    topBoundingVec = topBoundingVec_;
    lowBoundingVec = lowBoundingVec_;
    boundingRadius = boundingRadius_;
    particleRadius = particleRadius_;
    distanceThresholdToCombine = distanceThresholdToCombine_;
    breakApartThreshold = breakApartThreshold_;
    randomVelocityThreshold = randomVelocityThreshold_;
    startingSize = startingSize_;
    combinationThreshold = combinationThreshold_;
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
    float distance;
    float breakApartChance;
    float distX;
    float distY;
    float distZ;
    bool hitWall;
    bool combinedThisTurn;
    // drawSphere(shader,pos, glm::vec3(10,10,10), 0, 0,redMatterTex);
    for (int i =0; i < redMatter.size(); i++){
        hitWall = false;
        combinedThisTurn = false;
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
       //Check if we need to combine particles
        for (int j =i+1; j < redMatter.size(); j++){
            // if (j == i){
            //     continue;
            // } 
            //Distance vector between point X and Y
            distX = fabs(redMatter[i].position.x - redMatter[j].position.x);
            distY = fabs(redMatter[i].position.y - redMatter[j].position.y);
            distZ = fabs(redMatter[i].position.z - redMatter[j].position.z);
            distance = sqrt((distX*distX)+(distY*distY)+(distZ*distZ));
            distance -= (redMatter[i].size/1.3);
            if (distance < distanceThresholdToCombine){
                float combineChance = dis(gen);
                // float combine
                if (combineChance > combinationThreshold){
                    combineParticles(&redMatter[i], &redMatter[j], j);
                    combinedThisTurn = true;
                    // continue;
                }
            }
        }
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
        redMatter[i].position = redMatter[i].position + (redMatter[i].velocity);
        // redMatter[i].velocity.y += dis(gen);
        // redMatter[i].velocity = glm::normalize(redMatter[i].velocity + noiseVelocity())/100.0f;
        redMatter[i].velocity = (redMatter[i].velocity);

        redMatter[i].forwardVector = glm::normalize(redMatter[i].position + redMatter[i].velocity);
        // redMatter[i].velocity = glm::normalize(redMatter[i].velocity + noiseVelocity());
        // if (combinedThisTurn){
        //     drawSphere(shader,pos + redMatter[i].position, glm::vec3(redMatter[i].size * 0.80,redMatter[i].size *1.20,redMatter[i].size), 270, 90,redMatterTex);
        // }
        // else {
        drawSphere(shader,pos + redMatter[i].position, glm::vec3(redMatter[i].size,redMatter[i].size,redMatter[i].size), 270, 90,redMatterTex);
        // }
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
void combineParticles(particle *p1, particle *p2, int p2Index) {
    p1->size = pow(pow(p1->size, 3) + pow(p2->size, 3), 0.3333);

    // float weight1 = pow(p1->size, 3) / (pow(p1->size, 3) + pow(p2->size, 3));
    float weight1 = pow(p1->size, 3) / (pow(p1->size, 3) + pow(p2->size, 3));

    float weight2 = 1.0f - weight1;
    p1->velocity = p1->velocity * weight1 + p2->velocity * weight2;

    // Smooth position change
    glm::vec3 averageForwardVector = ((p2->forwardVector * weight1) + (p1->forwardVector* weight1)) / 2.0f;
    glm::vec3 targetPosition = p1->position - averageForwardVector;
    p1->position = glm::mix(p1->position, targetPosition, 0.1f);  // Smoothing factor 0.1f
    
    redMatter.erase(redMatter.begin() + p2Index);
    return;
}

void separateParticle(particle *p1) {
    particle p2;
    p1->size = pow(pow(p1->size, 3) / 2, 0.3333);
    p2.size = p1->size;

    // Smooth position separation
    p2.position = glm::mix(p1->position, p1->position - p1->forwardVector, 0.4f);
    
    // Smooth velocity separation
    p2.velocity = glm::mix(p1->velocity, p1->velocity / 10.0f + noiseVelocity() * 10.0f, 0.8f);

    redMatter.emplace_back(p2);
    return;
}

/*
    TODO: Smooth out combinations. Particles are too "jumpy" when they combine. Also, find a way to "combine" without exceeding the bounds of the 
*/
// void combineParticles(particle *p1, particle *p2, int p2Index){
//     p1->size = pow(pow(p1->size,3) + pow(p2->size,3),0.3333);
//     p1->velocity = (p1->velocity + p2->velocity)/2.0f;
//     // p1->velocity = glm::normalize(glm::cross(p1->velocity, p2->velocity));
//     glm::vec3 averageForwardVector = (p2->forwardVector + p1->forwardVector)/2.0f;
//     //This might take us out of bounds, but it works for separating so maybe not?
//     p1->position = p1->position - averageForwardVector;
    
//     // p1->position = p1->position;

//     redMatter.erase(redMatter.begin() + p2Index);
//     return;
// }
// void separateParticle(particle *p1){
//     particle p2;
//     p1->size = pow(pow(p1->size,3)/2, 0.3333);

//     //this could break some shit (Might go out of the box, we will see
//     p2.size = p1->size;
//     // p2.position = (p1->position + (p1->position + p1->forwardVector))/2.0f;
//     p2.position = (p1->position - p1->forwardVector);
//     // p2.position = (p1->position);


//     p2.velocity = (p1->velocity/10.0f + noiseVelocity() * 10.0f);
//     // p2.velocity.y += 0.01;
//     redMatter.emplace_back(p2);
//     return;
// }