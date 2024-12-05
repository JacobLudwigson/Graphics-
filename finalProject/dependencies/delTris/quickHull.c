#include "quickHull.h"
void QUICKHULL::Quickhull(){

}
void QUICKHULL::~Quickhull(){
    
}
void pushFaceToDrawArr(Face f){
    
    readyToDraw.push_back(f.a.x);
    readyToDraw.push_back(f.a.y);
    readyToDraw.push_back(f.a.z);
    readyToDraw.push_back(f.a.x);
    readyToDraw.push_back(f.a.y);
    readyToDraw.push_back(f.a.z);

}
glm::vec3 QUICKHUL::computeSurfaceNormal(Face f) {
    // Compute two edges of the triangle
    glm::vec3 A,B,C;
    A = f.a;
    B = f.b;
    C = f.c;
    glm::vec3 edge1 = B - A;
    glm::vec3 edge2 = C - A;

    // Compute the cross product of the two edges
    glm::vec3 normal = glm::cross(edge1, edge2);

    // Normalize the result to ensure the normal is a unit vector
    return glm::normalize(normal);
}
void QUICKHULL::calcFacesFromTetra(Tetrahedron tetra, Face* faceArr){
    faceArr[0] = {tetra.a, tetra.b, tetra.c}; // This is the base triangle we found before;
    faceArr[1] = {tetra.a, tetra.b, tetra.d};
    faceArr[2] = {tetra.b, tetra.c, tetra.d};
    faceArr[3] = {tetra.a, tetra.c, tetra.d};
    return;
}
bool QUICKHULL::isInFrontOfFace(Face f, glm::vec3 p){
    glm::vec3 surfaceNorm = computeSurfaceNormal(f);
    //If the normal is inverted (this algorithm doesnt consider wind order)
    if (glm::dot(surfaceNorm, middlePoint) < 0){
        surfaceNorm = -surfaceNorm;
    }
    if (glm::dot(surfaceNorm, f.a - p) > 0) return true;
    if (glm::dot(surfaceNorm, f.a - p) > 0) return true;
    if (glm::dot(surfaceNorm, f.a - p) > 0) return true;
    return 
}
//I could maybe do this as a macro? We will see. I think because of sqrt it needs to be a function
float QUICKHULL::distance(glm::vec3 p1, glm::vec3 p2){
    return sqrt(((p2.x - p1.x) * (p2.x - p1.x)) + 
                ((p2.y - p1.y) * (p2.y - p1.y)) + 
                ((p2.z - p1.z) * (p2.z - p1.z)));
}
glm::vec3 QUICKHULL::calcMeanOfTetraVectors(Tetrahedron tetra){
    return (tetra.a + tetra.b + tetra.c + tetra.d)/4.0f
}
//In this phase, 
//1. We need to find the 6 extreme points (EP) the min/max X, min/max Y, and min/max Z.
//2. The most distance two of these EP points build the base-line of the base triangle
//3. The most distant point of the EPs is the 3rd point of the base triangle
//4. The most distant point to the newly created base triangle becomes the pyramids apex
//5. Construct the first pyramid
//6. Update the face points set with the points that are "looking at" the face 
//   -(this should just be the dot product of the triangle normal and the point is positive right?)
//7. Push the 4 faces on the stack
void QUICKHULL::initQuickhull(){
    glm::vec3 minX, maxX, minY, maxY, minZ, maxZ;
    //May consider, depending on time complexity refactoring this code to work with a sorted list of points.
    for (auto& i : points){
        if (i.x < minX.x) minx = i;
        else if (i.x > maxX.x) maxX = i;
        if (i.x < minY.y) minY = i;
        else if (i.x > maxY.y) maxX = i;
        if (i.x < minZ.z) minZ = i;
        else if (i.x < maxX.x) maxX = i;
    }
    // 2. Determine Base Line
    Point baseLine1, baseLine2;
    float maxDistance = 0;
    for (glm::vec3 p1 : {minX, maxX, minY, maxY, minZ, maxZ}) {
        for (glm::vec3 p2 : {minX, maxX, minY, maxY, minZ, maxZ}) {
            float dist = distance(p1, p2);
            if (dist > maxDistance) {
                baseLine1 = p1;
                baseLine2 = p2;
                maxDistance = dist;
            }
        }
    }

    // 3. Find Third Point for Base Triangle - I am going to assume this is the point farthest from the mid-point of the baseline?
    glm::vec3 thirdPoint = findPointFarthestFromLine(baseLine1, baseLine2, points);

    // 4. Find Apex
    glm::vec3 apex = findPointFarthestFromTriangle(baseLine1, baseLine2, thirdPoint, points);

    // 5. Construct Initial Tetrahedron
    tetra = Tetrahedron(baseLine1, baseLine2, thirdPoint, apex);
    middlePoint = calcMeanOfTetraVectors(tetra);

    Face tetraFaces[4];
    calcFacesFromTetra(tetra, faceArr);
    

    for (glm::vec3& p : points) {
        if (isInFrontOfFace(tetraFaces[0], p)) facePointSets[i].push_back(p);
        else if (isInFrontOfFace(tetraFaces[1], p)) facePointSets[i].push_back(p);
        else if (isInFrontOfFace(tetraFaces[2], p)) facePointSets[i].push_back(p);
        else if (isInFrontOfFace(tetraFaces[3], p)) facePointSets[i].push_back(p);
    }
    for (int i = 0; i < 4; i++) {
        if (!facePointSets[i].isEmpty()){
            facePointSets.push_back(tetraFaces[i]);
        }
    }
}
/*
    Iterative Phase
    1. If stack is not empty, pop face off of stack
    2. Get most distant point of the face's point set.
    3. Find all the faces that can be seen from that points. Those faces must be adjacent to the current face
    4. Extract horizon edges of light faces and extrude to point
    5. Assign all points off all light faces to the new created faces
    6. Push new created faces on the stack, and start at 1. Do not push faces with no facePointSets 
*/
void QUICKHULL::runQuickHull(){
    int indexInPointsArr;
    while (!faceStack.empty()){
        Face currFace = faceStack.top()
        faceStack.pop();
    }
}
void QUICKHULL::setHull(){
    glGenVertexArrays(1, &VAOQ);
    glGenBuffers(1, &VBOQ);

    glBindVertexArray(VAOQ);
    glBindBuffer(GL_ARRAY_BUFFER, VBOQ);
    glBufferData(GL_ARRAY_BUFFER, readyToDraw.size() * sizeof(float), readyToDraw, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}
void QUICKHULL::drawHull(Gluint shader, glm::vec3 position, glm::vec3 scalar, float ph, float th, unsigned int texture){
    glUseProgram(shader);
    GLint textureLoc = glGetUniformLocation(shader, "image");

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture);
    glUniform1i(textureLoc, 0);

    model(shader, position, ph,th, scalar);
    
    glBindVertexArray(VAOQ);
    glDrawArrays(GL_TRIANGLES, 0, (readyToDraw.size() * sizeof(float))/3);
    glBindVertexArray(0);
}
