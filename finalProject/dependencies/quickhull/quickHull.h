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
//I had chatGPT help me with these two vec3 structs
struct Vec3Comparator {
    bool operator()(const glm::vec3& lhs, const glm::vec3& rhs) const {
        // Compare lexicographically based on x, then y, then z
        if (lhs.x != rhs.x) return lhs.x < rhs.x;
        if (lhs.y != rhs.y) return lhs.y < rhs.y;
        return lhs.z < rhs.z;
    }
};
struct Vec3Hasher {
    size_t operator()(const glm::vec3& v) const {
        size_t h1 = std::hash<float>{}(v.x);
        size_t h2 = std::hash<float>{}(v.y);
        size_t h3 = std::hash<float>{}(v.z);

        // Combine the three hashes using bit shifting and XOR
        return h1 ^ (h2 << 1) ^ (h3 << 2);
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
    unsigned int numIterations = 50;
    glm::vec3 computeSurfaceNormal(Face f);
    glm::vec3 computeCorrectedNormal(Face f, glm::vec3 insidePt);
    void pushFaceToDrawArr(Face f);
    void calcFacesFromTetra(Tetrahedron& tetra, Face* faceArr);
    bool isInFrontOfFace(Face &f, glm::vec3 p);
    float distance(glm::vec3 p1, glm::vec3 p2);
    glm::vec3 calcMeanOfTetraVectors(Tetrahedron tetra);
    glm::vec3 findPointFarthestFromTriangle(Face f, std::vector<glm::vec3>& pointsSet);
    void initQuickhull();
    bool areVectorsEqual(const glm::vec3& v1, const glm::vec3& v2, float epsilon = 1e-5f);
    void updateHorizonPoints(Face &f1, Face &f2, std::set<glm::vec3, Vec3Comparator> &uniquePoints);
    void runQuickHull();
    void cleanQuickHull();

    void setHull();
    void drawHull(GLuint shader, glm::vec3 position, glm::vec3 scalar, float ph, float th, unsigned int texture);
};


