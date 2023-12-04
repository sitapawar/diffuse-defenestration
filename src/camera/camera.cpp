#include <stdexcept>
#include "camera.h"

Camera::Camera(){};

glm::mat4 Camera::getViewMatrix() const {
    return viewMat;
}

void Camera::updateCamera(glm::vec3 pos, glm::vec3 look, glm::vec3 up, float heightAng,
                          float focalL, float aperture, int width,
                          int height, float near, float far){
        m_pos = pos;
        m_look = look;
        m_up = up;
        m_heightAng = heightAng;
        m_focalL = focalL;
        m_aperture = aperture;
        m_width = width;
        m_height = height;
        viewMat = calcViewMat();
        inverseViewMat = glm::inverse(viewMat);
        aspRat =  static_cast<float>(width) / static_cast<float>(height);
        m_near = near;
        m_far = far;
        projMat = calcProjMat();

}

void Camera::updateProjMat(float near, float far){
    m_near = near;
    m_far = far;
    projMat = calcProjMat();
}

glm::mat4 Camera::getProjMat() const {
    return projMat;
}

glm::mat4 Camera::calcProjMat(){
    //realtime II slide 23 - 38
    widthAng = m_heightAng * (static_cast<float>(m_width) /static_cast<float>(m_height));
    float halfThetaH = .5 * m_heightAng;
    float halfThetaW= .5 * widthAng;;

    float c = -m_near/m_far;

    glm::mat4 scalingMat(0.0f);
    // Construct the perspective matrix
    scalingMat[0][0] = 1.0f / (m_far * halfThetaW);
    scalingMat[1][1] = 1.0f / (m_far * halfThetaH);
    scalingMat[2][2] = 1.0f / m_far;
    scalingMat[3][3] = 1.0f;

    glm::mat4 perspective(0.0f);
    perspective[0][0] = 1.0f;
    perspective[1][1] = 1.0f;
    perspective[2][2] = 1.0f / (1+c);
    perspective[2][3] = -1.0f;
    perspective[3][2] = -c / (1+c);

    glm::mat4 glMat(0.0f);
    glMat[0][0] = 1.0f;
    glMat[1][1] = 1.0f;
    glMat[2][2] = -2.0f;
    glMat[3][2] = -1.0f;
    glMat[3][3] = 1.0f;
    return glMat*perspective*scalingMat;
}


glm::mat4 Camera::calcViewMat() const {
    glm::vec3 w = -glm::normalize(m_look);
    glm::vec3 v = glm::normalize(m_up - glm::dot(m_up, w)*w);
    glm::vec3 u = glm::cross(v,w);

    glm::vec4 rotCol0(u[0], v[0], w[0], 0.f);
    glm::vec4 rotCol1(u[1], v[1], w[1], 0.f);
    glm::vec4 rotCol2(u[2], v[2], w[2], 0.f);
    glm::vec4 rotCol3(0.f, 0.f, 0.f, 1.f);

    glm::vec4 posCol0(1.f, 0.f, 0.f, 0.f);
    glm::vec4 posCol1(0.f, 1.f, 0.f, 0.f);
    glm::vec4 posCol2(0.f, 0.f, 1.f, 0.f);
    glm::vec4 posCol3(-m_pos[0], -m_pos[1], -m_pos[2], 1.f);

    return glm::mat4(rotCol0,rotCol1,rotCol2,rotCol3) * glm::mat4(posCol0,posCol1,posCol2,posCol3);
}

glm::mat4 Camera::updateViewMat(glm::vec3 pos, glm::vec3 look, glm::vec3 up) {
    m_pos = pos;
    m_look = look;
    m_up = up;

    glm::vec3 w = -glm::normalize(m_look);
    glm::vec3 v = glm::normalize(m_up - glm::dot(m_up, w)*w);
    glm::vec3 u = glm::cross(v,w);

    glm::vec4 rotCol0(u[0], v[0], w[0], 0.f);
    glm::vec4 rotCol1(u[1], v[1], w[1], 0.f);
    glm::vec4 rotCol2(u[2], v[2], w[2], 0.f);
    glm::vec4 rotCol3(0.f, 0.f, 0.f, 1.f);

    glm::vec4 posCol0(1.f, 0.f, 0.f, 0.f);
    glm::vec4 posCol1(0.f, 1.f, 0.f, 0.f);
    glm::vec4 posCol2(0.f, 0.f, 1.f, 0.f);
    glm::vec4 posCol3(-m_pos[0], -m_pos[1], -m_pos[2], 1.f);

    viewMat = glm::mat4(rotCol0,rotCol1,rotCol2,rotCol3) * glm::mat4(posCol0,posCol1,posCol2,posCol3);
    inverseViewMat = glm::inverse(viewMat);
    return viewMat;
}


glm::mat4 Camera::getViewMatrixInverse() const {
    return inverseViewMat;
}

float Camera::getAspectRatio() const {
    return aspRat;
}

float Camera::getHeightAngle() const {
    return m_heightAng;
}

float Camera::getFocalLength() const {
    return m_focalL;
}

float Camera::getAperture() const {
    return m_aperture;
}

glm::vec4 Camera::getPos() const {
    return glm::vec4(m_pos, 1.0f);
}

