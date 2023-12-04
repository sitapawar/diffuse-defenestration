#include "Cube.h"

void Cube::updateParams(int param1) {
    m_vertexData = std::vector<float>();
    m_param1 = param1;
    setVertexData();
}

void Cube::makeTile(glm::vec3 topLeft,
                    glm::vec3 topRight,
                    glm::vec3 bottomLeft,
                    glm::vec3 bottomRight) {
    // Task 2: create a tile (i.e. 2 triangles) based on 4 given points.
//    insertVec3(m_vertexData, topLeft);

    glm::vec3 normal1 = glm::normalize(glm::cross(topLeft-bottomLeft,bottomLeft-bottomRight));
    insertVec3(m_vertexData, topLeft);
//    insertVec3(m_vertexData, glm::vec3(0,0,1));
    insertVec3(m_vertexData, normal1);
    insertVec3(m_vertexData, bottomLeft);
    insertVec3(m_vertexData, normal1);
    insertVec3(m_vertexData, bottomRight);
    insertVec3(m_vertexData, normal1);

    glm::vec3 normal2 = glm::normalize(glm::cross(topRight-bottomRight,topLeft-topRight));
    insertVec3(m_vertexData, topLeft);
    insertVec3(m_vertexData, normal2);
    insertVec3(m_vertexData, bottomRight);
    insertVec3(m_vertexData, normal2);
    insertVec3(m_vertexData, topRight);
    insertVec3(m_vertexData, normal2);

}

void Cube::makeFace(glm::vec3 topLeft,
                    glm::vec3 topRight,
                    glm::vec3 bottomLeft,
                    glm::vec3 bottomRight) {
    // Task 3: create a single side of the cube out of the 4
    //         given points and makeTile()
    // Note: think about how param 1 affects the number of triangles on
    //       the face of the cube
    //use m_param1 to determine how many tiles makes up the face
//     Check if m_param1 is greater than 1
    float param = m_param1;
    if (m_param1 > 1) {
        // Calculate the step size for dividing the face into tiles
        glm::vec3 offsetHor = (topRight-topLeft)/(param);
        glm::vec3 offsetVert = (bottomLeft-topLeft)/(param);

        // Loop through rows and columns to create tiles
        for (int i = 0.f; i < m_param1; i++) {
            for (int j = 0.f; j < m_param1; j++) {
            // Calculate the corners of the current tile
                glm::vec3 tileTopLeft = topLeft + static_cast<float>(i)* offsetHor + static_cast<float>(j)* offsetVert;
                glm::vec3 tileTopRight = tileTopLeft + offsetHor;
                glm::vec3 tileBottomLeft = tileTopLeft + offsetVert;
                glm::vec3 tileBottomRight = tileBottomLeft + offsetHor;

                Cube::makeTile(tileTopLeft, tileTopRight, tileBottomLeft, tileBottomRight);
            }
        }
    } else {
        // If m_param1 is 1 or less, create a single tile for the face
        Cube::makeTile(topLeft, topRight, bottomLeft, bottomRight);
    }
}

void Cube::setVertexData() {
    // Uncomment these lines for Task 2, then comment them out for Task 3:

//     makeTile(glm::vec3(-0.5f,  0.5f, 0.5f),
//              glm::vec3( 0.5f,  0.5f, 0.5f),
//              glm::vec3(-0.5f, -0.5f, 0.5f),
//              glm::vec3( 0.5f, -0.5f, 0.5f));

    // Uncomment these lines for Task 3:
    //front face
     makeFace(glm::vec3(-0.5f,  0.5f, 0.5f),
              glm::vec3( 0.5f,  0.5f, 0.5f),
              glm::vec3(-0.5f, -0.5f, 0.5f),
              glm::vec3( 0.5f, -0.5f, 0.5f));

//     Task 4: Use the makeFace() function to make all 6 sides of the cube
     //top face
     makeFace(glm::vec3(-0.5f,  0.5f, -0.5f),
              glm::vec3( 0.5f,  0.5f, -0.5f),
              glm::vec3(-0.5f, 0.5f, 0.5f),
              glm::vec3( 0.5f, 0.5f, 0.5f));

     //bottom face
     makeFace(glm::vec3( 0.5f,  -0.5f, -0.5f),glm::vec3(-0.5f,  -0.5f, -0.5f),
              glm::vec3( 0.5f, -0.5f, 0.5f),glm::vec3(-0.5f, -0.5f, 0.5f));

     //back face
     makeFace(glm::vec3( 0.5f,  0.5f, -0.5f), glm::vec3(-0.5f,  0.5f, -0.5f),
              glm::vec3( 0.5f, -0.5f, -0.5f),glm::vec3(-0.5f, -0.5f, -0.5f));

     //left side face
     makeFace(glm::vec3( -0.5f,  0.5f, -0.5f), glm::vec3(-0.5f,  0.5f, 0.5f),
              glm::vec3( -0.5f, -0.5f, -0.5f),glm::vec3(-0.5f, -0.5f, 0.5f));

     //right side face
     makeFace(glm::vec3( 0.5f,  0.5f, 0.5f), glm::vec3(0.5f,  0.5f,- 0.5f),
              glm::vec3( 0.5f, -0.5f, 0.5f),glm::vec3(0.5f, -0.5f, -0.5f));
}

// Inserts a glm::vec3 into a vector of floats.
// This will come in handy if you want to take advantage of vectors to build your shape!
void Cube::insertVec3(std::vector<float> &data, glm::vec3 v) {
    data.push_back(v.x);
    data.push_back(v.y);
    data.push_back(v.z);
}
