#include "Cylinder.h"

void Cylinder::updateParams(int param1, int param2) {
    m_vertexData = std::vector<float>();
    m_param1 = param1;
    m_param2 = param2;
    setVertexData();
}

// takes vector in spherical coordinates, returns cartesian
glm::vec3 Cylinder::cylToCartesian(glm::vec3 v) {
    return glm::vec3(v[0]*cos(v[1]), v[2], v[0]*sin(v[1]));
//    return glm::vec3(x,y,z);
}

glm::vec3 Cylinder::calcNormals(glm::vec3 pos) {
    return normalize(glm::vec3(pos[0],0,pos[2]));
}

void Cylinder::makeTile(glm::vec3 topLeft,
                        glm::vec3 topRight,
                        glm::vec3 bottomLeft,
                        glm::vec3 bottomRight,
                        bool face) {
    // Task 2: create a tile (i.e. 2 triangles) based on 4 given points.
    // triangle 1
    insertVec3(m_vertexData, topLeft, true);
    insertVec3(m_vertexData, (face) ? glm::vec3(0,(topLeft[1] > 0) ? 1 : -1 ,0) : calcNormals(topLeft), false);
    insertVec3(m_vertexData, bottomLeft, true);
    insertVec3(m_vertexData, (face) ? glm::vec3(0,(topLeft[1] > 0) ? 1 : -1 ,0) : calcNormals(bottomLeft), false);
    insertVec3(m_vertexData, bottomRight, true);
    insertVec3(m_vertexData, (face) ? glm::vec3(0,(topLeft[1] > 0) ? 1 : -1 ,0) : calcNormals(bottomRight), false);
    // triangle 2
    insertVec3(m_vertexData, topLeft, true);
    insertVec3(m_vertexData, (face) ? glm::vec3(0,(topLeft[1] > 0) ? 1 : -1 ,0) : calcNormals(topLeft), false);
    insertVec3(m_vertexData, bottomRight, true);
    insertVec3(m_vertexData, (face) ? glm::vec3(0,(topLeft[1] > 0) ? 1 : -1 ,0) : calcNormals(bottomRight), false);
    insertVec3(m_vertexData, topRight, true);
    insertVec3(m_vertexData, (face) ? glm::vec3(0,(topLeft[1] > 0) ? 1 : -1 ,0) : calcNormals(topRight), false);
}

void Cylinder::makeFace(float y) {
    // face is composed of m_param2 triangles
    float thetaStep = (M_PI*2) / m_param2;

    glm::vec3 n = glm::vec3(0, (y > 0) ? 1 : -1, 0);
    // make center triangle
    for (float theta = 0; theta < 2*M_PI; theta += thetaStep) {
        // generate the three vertices of the triangle
        glm::vec3 v1 = cylToCartesian(glm::vec3(0.5 / m_param1, theta, y));
        // ensure proper counterclockwise ordering of vertices
        glm::vec3 v2 = cylToCartesian(glm::vec3(0.5 / m_param1, (y < 0) ? theta + thetaStep : theta - thetaStep, y));
        // final vertex is just origin at proper y value
        glm::vec3 v3 = cylToCartesian(glm::vec3(0, theta, y));

        //true if its a vertex, false if it's a normal
        insertVec3(m_vertexData, v1, true);
        insertVec3(m_vertexData, n, false);
        insertVec3(m_vertexData, v2, true);
        insertVec3(m_vertexData, n, false);
        insertVec3(m_vertexData, v3, true);
        insertVec3(m_vertexData, n, false);
    }
    // make outer trapezoids
    float radStep = 0.5 / m_param1;
    for (float theta = 0; theta < 2*M_PI; theta += thetaStep) {
        for (int i = 0; i < m_param1 - 1; i++) {
            float outerRad = 0.5 - radStep*i;
            float innerRad = 0.5 - radStep*(i+1);
            glm::vec3 topLeft = cylToCartesian(glm::vec3(innerRad, theta, y));
            glm::vec3 bottomLeft = cylToCartesian(glm::vec3(outerRad, theta, y));
            glm::vec3 topRight = cylToCartesian(glm::vec3(innerRad, (y < 0) ? theta + thetaStep : theta - thetaStep, y));
            glm::vec3 bottomRight = cylToCartesian(glm::vec3(outerRad, (y < 0) ? theta + thetaStep : theta - thetaStep, y));
            makeTile(topLeft, topRight, bottomLeft, bottomRight, true);
        }
    }
}

void Cylinder::setVertexData() {
    // TODO for Project 5: Lights, Camera
    // top
    makeFace(0.5);
    // bottom
    makeFace(-0.5);
    // m_param2 sides
    float yStep = 1.f / m_param1;
    float thetaStep = (M_PI*2) / m_param2;
    for (float theta = 0; theta < 2*M_PI; theta += thetaStep) {
        for (int i = 0; i < m_param1; i++) {
            float botY = i*yStep - 0.5;
            float topY = (i+1)*yStep - 0.5;
            glm::vec3 topLeft = cylToCartesian(glm::vec3(m_radius, theta, topY));
            glm::vec3 bottomLeft = cylToCartesian(glm::vec3(m_radius, theta, botY));
            glm::vec3 topRight = cylToCartesian(glm::vec3(m_radius, theta - thetaStep, topY));
            glm::vec3 bottomRight = cylToCartesian(glm::vec3(m_radius, theta - thetaStep, botY));
            makeTile(topLeft, topRight, bottomLeft, bottomRight, false);
        }
    }
}

// Inserts a glm::vec3 into a vector of floats.
// This will come in handy if you want to take advantage of vectors to build your shape!
void Cylinder::insertVec3(std::vector<float> &data, glm::vec3 v, bool isPos) {
    // move vector into world space via ctm
    glm::vec4 worldSpaceVec = glm::vec4(v, (isPos) ? 1 : 0);
    if (!isPos) worldSpaceVec = normalize(worldSpaceVec);
    // store float data
    data.push_back(worldSpaceVec.x);
    data.push_back(worldSpaceVec.y);
    data.push_back(worldSpaceVec.z);
}
