#include "Sphere.h"
#include <vector>


void Sphere::updateParams(int param1, int param2) {
    m_vertexData = std::vector<float>();
    m_param1 = param1;
    m_param2 = param2*2;
    setVertexData();
}

// takes vector in spherical coordinates, returns cartesian
glm::vec3 sphereicalToCartesian(glm::vec3 v) {
    return glm::vec3(v[0]*sin(v[2])*cos(v[1]), v[0]*sin(v[2])*sin(v[1]), v[0]*cos(v[2]));
}


void pushVec3(glm::vec4 vec, std::vector<float>* data)
{
    data->push_back(vec.x);
    data->push_back(vec.y);
    data->push_back(vec.z);
}

void Sphere::makeTile(glm::vec3 topLeft,
                      glm::vec3 topRight,
                      glm::vec3 bottomLeft,
                      glm::vec3 bottomRight) {
    // Task 5: Implement the makeTile() function for a Sphere
    // Note: this function is very similar to the makeTile() function for Cube,
    //       but the normals are calculated in a different way!
    glm::vec3 center = glm::vec3(0,0,0);
    //    glm::vec3 normal1 = glm::normalize(glm::cross(topLeft-bottomLeft,bottomLeft-bottomRight));
    insertVec3(m_vertexData, topLeft);
    insertVec3(m_vertexData, glm::normalize(topLeft));
    insertVec3(m_vertexData, bottomLeft);
    insertVec3(m_vertexData, glm::normalize(bottomLeft));
    insertVec3(m_vertexData, bottomRight);
    insertVec3(m_vertexData, glm::normalize(bottomRight));

    //    glm::vec3 normal2 = glm::normalize(glm::cross(topRight-bottomRight,topLeft-topRight));
    insertVec3(m_vertexData, topLeft);
    insertVec3(m_vertexData, glm::normalize(topLeft));
    insertVec3(m_vertexData, bottomRight);
    insertVec3(m_vertexData, glm::normalize(bottomRight));
    insertVec3(m_vertexData, topRight);
    insertVec3(m_vertexData, glm::normalize(topRight));
}


void Sphere::makeWedge(float currentTheta, float nextTheta) {
    float wedgeHeightAngle = glm::radians(180.f / m_param1);
    for(float heightStart = 0; heightStart < M_PI; heightStart += wedgeHeightAngle) {
        glm::vec3 topLeft = sphereicalToCartesian(glm::vec3(0.5, currentTheta, heightStart));
        glm::vec3 topRight = sphereicalToCartesian(glm::vec3(0.5, nextTheta, heightStart));
        glm::vec3 bottomLeft = sphereicalToCartesian(glm::vec3(0.5, currentTheta, heightStart+wedgeHeightAngle));
        glm::vec3 bottomRight = sphereicalToCartesian(glm::vec3(0.5, nextTheta, heightStart+wedgeHeightAngle));
        makeTile(topLeft, topRight, bottomLeft, bottomRight);
    }
}


void Sphere::makeSphere() {
    float wedgeWidthAngle = (M_PI*2) / m_param2;
    for(float theta = 0; theta < M_PI*2; theta += wedgeWidthAngle) {
        makeWedge(theta, theta + wedgeWidthAngle);
    }
}

void Sphere::setVertexData() {
     makeSphere();
}

// Inserts a glm::vec3 into a vector of floats.
// This will come in handy if you want to take advantage of vectors to build your shape!
void Sphere::insertVec3(std::vector<float> &data, glm::vec3 v) {
    data.push_back(v.x);
    data.push_back(v.y);
    data.push_back(v.z);
}
