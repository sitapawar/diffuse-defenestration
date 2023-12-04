#include "Cone.h"

void Cone::updateParams(int param1, int param2) {
    m_vertexData = std::vector<float>();
    m_param1 = param1;
    m_param2 = param2;
    setVertexData();
}

// takes vector in spherical coordinates, returns cartesian
glm::vec3 Cone::cylToCartesian(glm::vec3 v) {
    return glm::vec3(v[0]*cos(v[1]), v[2], v[0]*sin(v[1]));
}

glm::vec3 Cone::calcNorm(glm::vec3 position) {
    // Ensure the cone is centered at the origin
    float normalizedX = position.x / sqrt(position.x * position.x + position.z * position.z);
    float normalizedZ = position.z / sqrt(position.x * position.x + position.z * position.z);

    // Calculate the normalized normal vector
    return normalize(glm::vec3(normalizedX, 1, normalizedZ));
}

void Cone::makeTile(glm::vec3 topLeft,
                    glm::vec3 topRight,
                    glm::vec3 bottomLeft,
                    glm::vec3 bottomRight,
                    bool face) {
    // Task 2: create a tile (i.e. 2 triangles) based on 4 given points.
    // triangle 1
    insertVec3(m_vertexData, topLeft, true);
    insertVec3(m_vertexData, (face) ? glm::vec3(0,-1,0) : calcNorm(topLeft), false);
    insertVec3(m_vertexData, bottomLeft, true);
    insertVec3(m_vertexData, (face) ? glm::vec3(0,-1,0) : calcNorm(bottomLeft), false);
    insertVec3(m_vertexData, bottomRight, true);
    insertVec3(m_vertexData, (face) ? glm::vec3(0,-1,0) : calcNorm(bottomRight), false);
    // triangle 2
    insertVec3(m_vertexData, topLeft, true);
    insertVec3(m_vertexData, (face) ? glm::vec3(0,-1,0) : calcNorm(topLeft), false);
    insertVec3(m_vertexData, bottomRight, true);
    insertVec3(m_vertexData, (face) ? glm::vec3(0,-1,0) : calcNorm(bottomRight), false);
    insertVec3(m_vertexData, topRight, true);
    insertVec3(m_vertexData, (face) ? glm::vec3(0,-1,0) : calcNorm(topRight), false);
}

void Cone::makeFace(float y) {
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

void Cone::setVertexData() {
    // TODO for Project 5: Lights, Camera
    // Base of the cone
    makeFace(-0.5);

    // Top of the cone
    float angleStep = (M_PI * 2) / m_param2;
    float heightStep = 1.f / m_param1;

    // Generate top triangles
    for (float angle = 0; angle < 2 * M_PI; angle += angleStep) {
        // Top vertex is the tip of the cone
        glm::vec3 tipVertex = cylToCartesian(glm::vec3(0, angle, 0.5));
        // Other two vertices are down heightStep, radius heightStep/2
        glm::vec3 bottomVertex1 = cylToCartesian(glm::vec3(heightStep / 2, angle, 0.5 - heightStep));
        glm::vec3 bottomVertex2 = cylToCartesian(glm::vec3(heightStep / 2, angle - angleStep, 0.5 - heightStep));

        // Insert vertices and normals
        insertVec3(m_vertexData, tipVertex, true);
        // For tip normal, generate a point horizontally centered on the face
        insertVec3(m_vertexData, calcNorm(cylToCartesian(glm::vec3(heightStep / 2, (angle + (angle - angleStep)) / 2, 0.5 - heightStep))), false);
        insertVec3(m_vertexData, bottomVertex1, true);
        insertVec3(m_vertexData, calcNorm(bottomVertex1), false);
        insertVec3(m_vertexData, bottomVertex2, true);
        insertVec3(m_vertexData, calcNorm(bottomVertex2), false);
    }

    // Generate trapezoids
    for (float angle = 0; angle < 2 * M_PI; angle += angleStep) {
        for (int i = 0; i < m_param1 - 1; i++) {
            float bottomY = i * heightStep - 0.5;
            float topY = (i + 1) * heightStep - 0.5;
            float bottomRadius = 0.5 - i * heightStep / 2;
            float topRadius = 0.5 - (i + 1) * heightStep / 2;

            // Define vertices for the trapezoid
            glm::vec3 topLeft = cylToCartesian(glm::vec3(topRadius, angle, topY));
            glm::vec3 bottomLeft = cylToCartesian(glm::vec3(bottomRadius, angle, bottomY));
            glm::vec3 topRight = cylToCartesian(glm::vec3(topRadius, angle - angleStep, topY));
            glm::vec3 bottomRight = cylToCartesian(glm::vec3(bottomRadius, angle - angleStep, bottomY));

            // Create the trapezoid
            makeTile(topLeft, topRight, bottomLeft, bottomRight, false);
        }
    }
}



// Inserts a glm::vec3 into a vector of floats.
// This will come in handy if you want to take advantage of vectors to build your shape!
void Cone::insertVec3(std::vector<float> &data, glm::vec3 v, bool position) {
    // move vector into world space via ctm
    glm::vec4 worldSpaceVec = glm::vec4(v, (position) ? 1 : 0);
    if (!position) worldSpaceVec = normalize(worldSpaceVec);
    // store float data
    data.push_back(worldSpaceVec.x);
    data.push_back(worldSpaceVec.y);
    data.push_back(worldSpaceVec.z);
}
