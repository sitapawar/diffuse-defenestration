#include "sceneparser.h"
#include "scenefilereader.h"
#include <glm/gtx/transform.hpp>

#include <chrono>
#include <iostream>

void dfSceneGraph(SceneNode* node, glm::mat4 CTM, RenderData &renderData) {
    if (node == nullptr) {
        return; //account for null ptrs
    }

    for (SceneTransformation* transform : node->transformations) {
        glm::mat4 m_matrix;
        switch(transform->type){
        case TransformationType::TRANSFORMATION_TRANSLATE:
            m_matrix = glm::translate(transform->translate);
            break;
        case TransformationType::TRANSFORMATION_ROTATE:
            m_matrix = glm::rotate(transform->angle, transform->rotate);
            break;
        case TransformationType::TRANSFORMATION_SCALE:
            m_matrix = glm::scale(transform->scale);
            break;
        case TransformationType::TRANSFORMATION_MATRIX:
            m_matrix = transform->matrix;
            break;
        }
        CTM = CTM * m_matrix;
    }

    for (ScenePrimitive* primitive : node->primitives) {
        RenderShapeData newShape;
        newShape.ctm = CTM;
        newShape.primitive.material = primitive->material;
        newShape.primitive.meshfile = primitive->meshfile;
        newShape.primitive.type = primitive->type;

        renderData.shapes.push_back(newShape);
    }

    for (SceneLight* light : node->lights) {
        SceneLightData newLight;
        newLight.angle = light->angle;
        newLight.color = light->color;
        newLight.dir = CTM*light->dir;
        newLight.function = light->function;
        newLight.height = light->height;
        newLight.id = light->id;
        newLight.penumbra = light->penumbra;
        newLight.type = light->type;
        newLight.width = light->width;
        newLight.pos = CTM*glm::vec4(0.f, 0.f, 0.f, 1.f);
        renderData.lights.push_back(newLight);
    }

    //recurse through children
    for (SceneNode* child : node->children) {
        dfSceneGraph(child, CTM, renderData);
    }
}

bool SceneParser::parse(std::string filepath, RenderData &renderData) {
    ScenefileReader fileReader = ScenefileReader(filepath);
    bool success = fileReader.readJSON();
    if (!success) {
        return false;
    }

    // TODO: Use your Lab 5 code here

    //  populate renderData with global data, and camera data;
    renderData.cameraData = fileReader.getCameraData();
    renderData.globalData = fileReader.getGlobalData();

    // populate renderData's list of primitives and their transforms
    SceneNode* myRoot = fileReader.getRootNode();
    renderData.shapes.clear();

    glm::mat4 ctm = glm::mat4(1.0f);
    dfSceneGraph(myRoot, ctm, renderData);

    return true;
}
