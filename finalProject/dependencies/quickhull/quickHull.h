#include "../shapes/halfCube.h"

// Basic Structures
struct Tetrahedron {glm::vec3 a,b,c,d;};
struct Face {
    glm::vec3 a,b,c;
    glm::vec3 normal;
    std::vector<glm::vec3> pointSet;
    bool toDelete = false;

    bool operator==(const Face& other) const {
        return (a == other.a && b == other.b && c == other.c);
    }
};


class QUICKHULL {
    private:
    std::vector<Face> mesh;
    std::vector <float> readyToDraw;
    glm::vec3 middlePoint;
    GLuint VBOQ, VAOQ;
    unsigned int sizeVertArr;

    public:
    QUICKHULL();
    ~QUICKHULL();
    std::vector <glm::vec3> points;
    GLuint shaderToDraw;
    unsigned int numIterations = 20;
    glm::vec3 computeSurfaceNormal(Face f);
    glm::vec3 computeCorrectedNormal(Face f, glm::vec3 insidePt);
    void pushFaceToDrawArr(Face f);
    void calcFacesFromTetra(Tetrahedron& tetra, Face* faceArr);
    bool isInFrontOfFace(Face &f, glm::vec3 p);
    glm::vec3 calcMeanOfTetraVectors(Tetrahedron tetra);
    glm::vec3 findPointFarthestFromTriangle(Face f, std::vector<glm::vec3>& pointsSet);
    void initQuickhull();
    bool areVectorsEqual(const glm::vec3& v1, const glm::vec3& v2, float epsilon = 1e-7f);
    void updateHorizonPoints(Face &f1, std::vector<Face> &nonLightFaces, std::vector<std::pair<glm::vec3, glm::vec3>> &horizonEdges);
    void runQuickHull();
    void cleanQuickHull();

    void setHull();
    void drawHull(GLuint shader, glm::vec3 position, glm::vec3 scalar, float ph, float th, unsigned int texture);
};


