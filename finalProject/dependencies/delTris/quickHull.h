#include "../particles/particle.h"

// Basic Structures
struct Tetrahedron {glm::vec3 a,b,c,d};
struct Face {glm::vec3 a,b,c};

class QUICKHULL {

    private:
    std::vector <glm::vec3> points;
    std::vector <Tetrahedron> tetrahedrons;
    std::vector <std::vector<glm::vec3>> facePointSets;
    std::stack <Face> faceStack;
    std::vector <float> readyToDraw;
    glm::vec3 middlePoint;
    GLuint VBOQ, VBOAQ;
    unsigned int sizeVertArr;

    public:
    void Quickhull();
    void ~Quickhull();
    glm::vec3 computeSurfaceNormal(Face f);
    void calcFacesFromTetra(Tetrahedron tetra, Face* faceArr);
    bool isInFrontOfFace(Face f, glm::vec3 p);
    float distance(glm::vec3 p1, glm::vec3 p2);
    glm::vec3 calcMeanOfTetraVectors(Tetrahedron tetra);
    void initQuickhull();
    void runQuickHull();
    void setHull();
    void drawHull(Gluint shader, glm::vec3 position, glm::vec3 scalar, float ph, float th, unsigned int texture);
}


