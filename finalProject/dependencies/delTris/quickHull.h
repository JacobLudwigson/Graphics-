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
    glm::vec3 middlePoint;
    public:
    void Quickhull();
    void ~Quickhull();

    void initQuickhull();
}


