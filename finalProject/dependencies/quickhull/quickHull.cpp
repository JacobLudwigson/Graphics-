#include "quickHull.h"
QUICKHULL::QUICKHULL(){

}
QUICKHULL::~QUICKHULL(){

}
void QUICKHULL::pushFaceToDrawArr(Face f){
        // drawSphere(shader,glm::vec3(0,4,93), glm::vec3(50,50,50), 270, 90,0);
    float arr[] = {
        f.a.x, f.a.y, f.a.z, f.normal.x,f.normal.y, f.normal.z, 0.0, 0.0,
        f.b.x, f.b.y, f.b.z, f.normal.x,f.normal.y, f.normal.z, 0.5, 1.0,
        f.c.x, f.c.y, f.c.z, f.normal.x,f.normal.y, f.normal.z, 1.0, 0.0
    };
    std::copy(std::begin(arr), std::end(arr), std::back_inserter(readyToDraw));
}
glm::vec3 QUICKHULL::computeSurfaceNormal(Face f) {
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
glm::vec3 QUICKHULL::computeCorrectedNormal(Face f, glm::vec3 insidePt) {
    glm::vec3 a = f.a;
    glm::vec3 b = f.b;
    glm::vec3 c = f.c;

    glm::vec3 normal = glm::normalize(glm::cross(b - a, c - a));
    if (glm::dot(normal, a - insidePt) > 0) {
        normal = -normal;
    }
    else if (glm::dot(normal, b - insidePt) > 0) {
        normal = -normal;
    }
    else if (glm::dot(normal, c - insidePt) > 0) {
        normal = -normal;
    }
    return normal;
}
void QUICKHULL::calcFacesFromTetra(Tetrahedron& tetra, Face* faceArr) {
    faceArr[0] = {.a = tetra.a, .b = tetra.b, .c = tetra.c};
    faceArr[1] = {.a = tetra.a, .b = tetra.c, .c = tetra.d};
    faceArr[2] = {.a = tetra.a, .b = tetra.d, .c = tetra.b};
    faceArr[3] = {.a = tetra.d, .b = tetra.c, .c = tetra.a};
}
bool QUICKHULL::isInFrontOfFace(Face &f, glm::vec3 p){
    if (glm::dot(f.normal, f.a - p) > 0.0f) return true;
    // if (glm::dot(f.normal, f.b - p) > 0.0f) return true;
    // if (glm::dot(f.normal, f.c - p) > 0.0f) return true;

    return false;
}
//I could maybe do this as a macro? We will see. I think because of sqrt it needs to be a function
float QUICKHULL::distance(glm::vec3 p1, glm::vec3 p2){
    return sqrt(((p2.x - p1.x) * (p2.x - p1.x)) + 
                ((p2.y - p1.y) * (p2.y - p1.y)) + 
                ((p2.z - p1.z) * (p2.z - p1.z)));
}
glm::vec3 QUICKHULL::calcMeanOfTetraVectors(Tetrahedron tetra){
    return (tetra.a + tetra.b + tetra.c + tetra.d)/4.0f;
}
glm::vec3 QUICKHULL::findPointFarthestFromTriangle(Face f, std::vector<glm::vec3> &pointsSet) 
{
    glm::vec3 apex = glm::vec3(-INFINITY, -INFINITY, -INFINITY);   // Farthest point
    float maxDistance = -1.0f; // Maximum perpendicular distance

    glm::vec3 baseLine1 = f.a;
    glm::vec3 baseline2 = f.b;
    glm::vec3 thirdPoint = f.c;

    for (const auto& point : pointsSet) {
        if (point == f.a || point == f.b || point == f.c){
            continue;
        }
        glm::vec3 vectorToPoint = point - baseLine1;

        float distance = glm::dot(vectorToPoint, f.normal);
    


        if (std::abs(distance) > maxDistance) {
            maxDistance = std::abs(distance);
            apex = point;
        }
    }
    return apex;

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
    readyToDraw.clear();
    glm::vec3 maxX = glm::vec3(-INFINITY, -INFINITY, -INFINITY);
    glm::vec3 maxY = glm::vec3(-INFINITY, -INFINITY, -INFINITY);
    glm::vec3 maxZ = glm::vec3(-INFINITY, -INFINITY, -INFINITY);
    glm::vec3 minX = glm::vec3(INFINITY, INFINITY, INFINITY);
    glm::vec3 minY = glm::vec3(INFINITY, INFINITY, INFINITY);
    glm::vec3 minZ = glm::vec3(INFINITY, INFINITY, INFINITY);
    // glm::vec3 sum = glm::vec3(0,0,0);
    // , maxX, minY, maxY, minZ, maxZ;
    //May consider, depending on time complexity refactoring this code to work with a sorted list of points.
    for (auto& i : points) {
        if (i.x < minX.x) minX = i;
        if (i.x > maxX.x) maxX = i;

        if (i.y < minY.y) minY = i;
        if (i.y > maxY.y) maxY = i;

        if (i.z < minZ.z) minZ = i;
        if (i.z > maxZ.z) maxZ = i;
        // sum+=i;
    }
    // sum /= points.size();

    // 2. Determine Base Line
    glm::vec3 baseLine1, baseLine2;
    float maxDistance = 0;
    for (glm::vec3 p1 : {minX, maxX, minY, maxY, minZ, maxZ}) {
        for (glm::vec3 p2 : {minX, maxX, minY, maxY, minZ, maxZ}) {
            if (p1 == p2) continue; // Skip same points
            float dist = glm::distance(p1, p2);
            if (dist > maxDistance) {
                baseLine1 = p1;
                baseLine2 = p2;
                maxDistance = dist;
            }
        }
    }

    // 3. Find Third Point
    glm::vec3 thirdPoint;
    float maxLineDistance = -1.0f;
    for (glm::vec3 p : {minX, maxX, minY, maxY, minZ, maxZ}) {
        if (p == baseLine1 || p == baseLine2) continue; // Skip baseline points

        // Calculate distance from point to the line
        glm::vec3 lineVec = baseLine2 - baseLine1;
        glm::vec3 pointVec = p - baseLine1;
        glm::vec3 projection = glm::dot(pointVec, lineVec) / glm::dot(lineVec, lineVec) * lineVec;
        glm::vec3 perpendicular = pointVec - projection;
        float distToLine = glm::length(perpendicular);

        if (distToLine > maxLineDistance) {
            thirdPoint = p;
            maxLineDistance = distToLine;
        }
    }

    // 4. Find Apex
    Face newFace = {
        .a = baseLine1,
        .b = baseLine2,
        .c = thirdPoint,
    };
    glm::vec3 apex = findPointFarthestFromTriangle(newFace, points);

    Tetrahedron tetra = {
        baseLine1, baseLine2, thirdPoint, apex
    };

    middlePoint = calcMeanOfTetraVectors(tetra);

    Face tetraFaces[4];
    calcFacesFromTetra(tetra, tetraFaces);
    for (int i = 0; i < 4; i++){
        tetraFaces[i].normal = computeCorrectedNormal(tetraFaces[i],middlePoint);
    }
    
    for (glm::vec3& p : points) {
        for (auto& it : tetraFaces) {
            if (isInFrontOfFace(it, p)) {
                if (areVectorsEqual(p,it.a) || areVectorsEqual(p,it.b)|| areVectorsEqual(p,it.c)) continue;
                else {it.pointSet.push_back(p);
                break;
                }
            }
        }
    }
    for (int i = 0; i < 4; i++){
        mesh.push_back(tetraFaces[i]);
    }
    // middlePoint = sum;
}
bool QUICKHULL::areVectorsEqual(const glm::vec3& v1, const glm::vec3& v2, float epsilon) {
    return glm::length(v1 - v2) < epsilon;
}
void QUICKHULL::updateHorizonPoints(Face &f1, Face &f2, std::set<glm::vec3, Vec3Comparator> &uniquePoints) {
    if (areVectorsEqual(f1.a, f2.a) || areVectorsEqual(f1.a, f2.b) || areVectorsEqual(f1.a, f2.c)) {
        uniquePoints.insert(f1.a);
    }
    if (areVectorsEqual(f1.b, f2.a) || areVectorsEqual(f1.b, f2.b) || areVectorsEqual(f1.b, f2.c)) {
        uniquePoints.insert(f1.b);
    }
    if (areVectorsEqual(f1.c, f2.a) || areVectorsEqual(f1.c, f2.b) || areVectorsEqual(f1.c, f2.c)) {
        uniquePoints.insert(f1.c);
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
    int indexInPointsArr, size1, size2;
    int deletedTriangles, addedTriangles;
    glm::vec3 meanOfCurrentPolygon = glm::vec3(0,0,0);
    int countActiveFaces;
    int maxIterations = 0; // we need to set the maximum iterations, because otherwise this algorithm will stew infinitately on point clouds that dont have a convex encapsulating polygon
    Face currFace;
    std::vector <Face> lightFaces;
    std::vector <Face> nonLightFaces;
    std::vector <glm::vec3> horizonPoints;
    std::set<glm::vec3, Vec3Comparator> uniquePoints;

    //Pop a face from the stack, access its facePointSet, and attempt to draw triangles from the most distant point.
    for (int it = 0; it < mesh.size(); it++){
        maxIterations += 1;
        if (maxIterations > 200){
            break;
        }
        currFace = mesh[it];
        if (currFace.toDelete || mesh[it].pointSet.empty()){
            continue;
        }
        
        meanOfCurrentPolygon = glm::vec3(0,0,0);
        lightFaces.clear();
        uniquePoints.clear();
        nonLightFaces.clear();
        glm::vec3 mostDistPt = findPointFarthestFromTriangle(currFace, mesh[it].pointSet);
        if (!isInFrontOfFace(mesh[it], mostDistPt)){
            printf("How did we get here?\n");
            exit(0);
        }
        for (auto& i : mesh){
            if (i.toDelete) continue;
            i.pointSet.clear();
            if (isInFrontOfFace(i, mostDistPt)){
                i.toDelete = true;
                lightFaces.push_back(i);
            }
            else{
                nonLightFaces.push_back(i);
            }
        }
        for (auto& i : lightFaces){
            for (auto& j : nonLightFaces){
                updateHorizonPoints(i, j,uniquePoints);
            }   
        }
        horizonPoints.assign(uniquePoints.begin(), uniquePoints.end());
        int horizSize = horizonPoints.size();
        if (horizSize < 2){
            printf("Light Faces: %d, Non-Light Faces: %d\n",lightFaces.size(), nonLightFaces.size());
            printf("Adding non convex shape!\n");
            exit(0);
        }
        int secondIndex;
        for (int i = 0; i < horizSize; i++){
            secondIndex = (i + 1) % horizSize;
            meanOfCurrentPolygon += (mostDistPt + horizonPoints[i] + horizonPoints[secondIndex]);
        }
        meanOfCurrentPolygon /= horizSize * 3;
        Face newFace;
        for (int i = 0; i < horizSize; i++){
            secondIndex = i+1;
            if (i == horizSize-1){
                secondIndex = 0;
            }
            newFace = {
                .a = mostDistPt,
                .b = horizonPoints[i],
                .c = horizonPoints[secondIndex],
                .toDelete = false,
            };
            newFace.normal = computeCorrectedNormal(newFace,meanOfCurrentPolygon);
            mesh.push_back(newFace);
        }
        for (glm::vec3& p : points) {
            for (int j = it; j < mesh.size(); j++) {
                if (mesh[j].toDelete) continue;
                if (isInFrontOfFace(mesh[j], p)) {
                    if (areVectorsEqual(p,mesh[j].a) || areVectorsEqual(p,mesh[j].b)|| areVectorsEqual(p,mesh[j].c)){
                        continue;
                    } 
                    else {mesh[j].pointSet.push_back(p);
                    break;
                    }
                }
            }   
        }
    }
    for (auto& face : mesh) {
        if (face.toDelete) continue;
        pushFaceToDrawArr(face);
    }
}
void QUICKHULL::cleanQuickHull(){
    mesh.clear();
    readyToDraw.clear();
}
void QUICKHULL::setHull(){
    glGenVertexArrays(1, &VAOQ);
    glGenBuffers(1, &VBOQ);

    glBindVertexArray(VAOQ);
    glBindBuffer(GL_ARRAY_BUFFER, VBOQ);
    glBufferData(GL_ARRAY_BUFFER, readyToDraw.size() * sizeof(float), readyToDraw.data(), GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}
void QUICKHULL::drawHull(GLuint shader, glm::vec3 position, glm::vec3 scalar, float ph, float th, unsigned int texture){
    glUseProgram(shader);
    GLint textureLoc = glGetUniformLocation(shader, "image");

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture);
    glUniform1i(textureLoc, 0);

    // model(shader, position, ph,th, scalar* 0.5f);
    model(shader, position, ph,th, scalar);

    
    glBindVertexArray(VAOQ);
    glDrawArrays(GL_TRIANGLES, 0, (readyToDraw.size()/8));
    glBindVertexArray(0);
}
