#include "particle.h"
void ParticleSystem::updateVelocity(float newV){
    velocity = newV;
}
void ParticleSystem::updateQhull(bool q){
    qhull = q;
}
void ParticleSystem::updateMove(int move_){
    move = move_;
}
void ParticleSystem::updateDrawParticles(bool b){
    drawParticle = b;
}
void ParticleSystem::updateDrawSizeMult(float m){
    drawSizeMult = m;
}

void ParticleSystem::setParamsParticles(glm::vec3 topBoundingVec_, glm::vec3 lowBoundingVec_,float boundingRadius_,
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

void ParticleSystem::initParticles(){
    particle source;
    source.size = startingSize;
    source.position = glm::vec3(0,0,0);
    source.velocity = noiseVelocity() * velocity;
    source.forwardVector = glm::normalize(source.position + source.velocity);
    redMatter.push_back(source);
}
void ParticleSystem::reverseVelocityYVector(particle *p){
    p->velocity.y = -p->velocity.y;
    p->forwardVector = glm::normalize(p->position + p->velocity);
}
void ParticleSystem::reverseXandZVectors(particle *p){
    float temp = p->velocity.y;
    glm::vec3 normalVector = -p->position/sqrt(p->position * p->position);
    p->velocity = p->velocity - (2.0f * (p->velocity * normalVector)) * normalVector;
    p->velocity.y = temp;
    p->forwardVector = glm::normalize(p->position + p->velocity);
}
void ParticleSystem::populatePointsVector(QUICKHULL *qHull){
    qHull->points.clear();
    int size = redMatter.size();
    for (int i = 0; i <size; i++){
        qHull->points.push_back(redMatter[i].position);
    }
}
void ParticleSystem::drawParticles(GLuint shader, unsigned int redMatterTex, glm::vec3 pos,QUICKHULL *qHull){
    static std::random_device rd;
    static std::mt19937 gen(rd());
    std::uniform_real_distribution<> dis(0, 1.0);

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
    for (int i =0; i < redMatter.size(); i++){
        if (move){
            hitWall = false;
            combinedThisTurn = false;
            if (redMatter.size() > 6){
                for (int j =i+1; j < redMatter.size(); j++){
                    distX = fabs(redMatter[i].position.x - redMatter[j].position.x);
                    distY = fabs(redMatter[i].position.y - redMatter[j].position.y);
                    distZ = fabs(redMatter[i].position.z - redMatter[j].position.z);
                    distance = sqrt((distX*distX)+(distY*distY)+(distZ*distZ));
                    distance -= (redMatter[i].size/1.3);
                    if (distance < distanceThresholdToCombine){
                        float combineChance = dis(gen);
                        if (combineChance > combinationThreshold){
                            combineParticles(&redMatter[i], &redMatter[j], j);
                            combinedThisTurn = true;
                        }
                    }
                }
            }
            if ((redMatter[i].position.y - (redMatter[i].size)) <= lowBoundY || (redMatter[i].position.y + (redMatter[i].size)) >= topBoundingY){
                hitWall = true;
                reverseVelocityYVector(&redMatter[i]);
            }
            distX = redMatter[i].position.x - lowBoundingVec.x; 
            distZ = redMatter[i].position.z - lowBoundingVec.z;

            float distanceFromCenter = sqrt(distX * distX + distZ * distZ);
            
            if (distanceFromCenter + redMatter[i].size > boundingRadius) {
                hitWall = true;            
                reverseXandZVectors(&redMatter[i]);
            }
            if (hitWall && redMatter[i].size > particleRadius){
                breakApartChance = dis(gen);
                if (breakApartChance > breakApartThreshold){
                    separateParticle(&redMatter[i]);
                }
            }        
            redMatter[i].position = redMatter[i].position + (redMatter[i].velocity);
            redMatter[i].velocity = (redMatter[i].velocity);

            redMatter[i].forwardVector = glm::normalize(redMatter[i].position + redMatter[i].velocity);
        }
        if (drawParticle){
            drawSphere(shader,pos + redMatter[i].position, glm::vec3(redMatter[i].size * drawSizeMult,redMatter[i].size* drawSizeMult,redMatter[i].size* drawSizeMult), 270, 90,redMatterTex);
        }
    }
    if (redMatter.size() > 5 && qhull){
        // drawSphere(shader,glm::vec3(0,4,93), glm::vec3(50,50,50), 270, 90,0);
        if (qHull->numIterations > 0){
        if (move){
            setMaterialUniforms(shader, fire);
            qHull->cleanQuickHull();
            populatePointsVector(qHull);
            qHull->initQuickhull();
            qHull->runQuickHull();
        }
        qHull->setHull();
        qHull->drawHull(shader, pos, glm::vec3(1,1,1), 0, 0,redMatterTex);
    }
    }
    return;
}
glm::vec3 ParticleSystem::noiseVelocity(){
    static std::random_device rd;
    static std::mt19937 gen(rd());
    std::uniform_real_distribution<> dis(-0.5, 0.5);
    float noiseVelocityX = dis(gen);
    float noiseVelocityY = (dis(gen));
    float noiseVelocityZ = dis(gen);
    return glm::vec3(noiseVelocityX/100,
                    noiseVelocityY/100,
                    noiseVelocityZ/100);
}
void ParticleSystem::combineParticles(particle *p1, particle *p2, int p2Index) {
    p1->size = pow(pow(p1->size, 3) + pow(p2->size, 3), 0.3333);

    float weight1 = pow(p1->size, 3) / (pow(p1->size, 3) + pow(p2->size, 3));

    float weight2 = 1.0f - weight1;
    p1->velocity = p1->velocity * weight1 + p2->velocity * weight2;

    glm::vec3 averageForwardVector = ((p2->forwardVector * weight1) + (p1->forwardVector* weight1)) / 2.0f;
    glm::vec3 targetPosition = p1->position - averageForwardVector;
    p1->position = glm::mix(p1->position, targetPosition, 0.1f);
    
    redMatter.erase(redMatter.begin() + p2Index);
    return;
}

void ParticleSystem::separateParticle(particle *p1) {
    particle p2;
    p1->size = pow(pow(p1->size, 3) / 2, 0.3333);
    p2.size = p1->size;

    p2.position = glm::mix(p1->position, p1->position - p1->forwardVector, 0.4f);
    
    p2.velocity = glm::mix(p1->velocity, p1->velocity / velocity + noiseVelocity() * velocity, 0.8f);

    redMatter.emplace_back(p2);
    return;
}