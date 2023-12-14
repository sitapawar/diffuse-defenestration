#ifndef MODEL_H
#define MODEL_H

#include <QImage>
#include <vector>
#include <GL/glew.h>
struct Model {
    int textureIndex;
    std::vector<float> modelData;
    QImage modelTexture;
    GLuint model_vbo;
    GLuint model_vao;
    GLuint textureID;

};

#endif // MODEL_H
