#pragma once

#include <glm/glm.hpp>

// A class representing a virtual camera.

// Feel free to make your own design choices for Camera class, the functions below are all optional / for your convenience.
// You can either implement and use these getters, or make your own design.
// If you decide to make your own design, feel free to delete these as TAs won't rely on them to grade your assignments.

class Camera {
public:
//    Camera(glm::vec3 pos, glm::vec3 look, glm::vec3 up, float heightAng, float focalL, float aperture,
//           int width, int height, float near, float far);
    Camera();

    // Returns the view matrix for the current camera settings.
    glm::mat4 getViewMatrix() const;
    // Returns the view inversematrix for the current camera settings.
    glm::mat4 getViewMatrixInverse() const;

    glm::mat4 updateViewMat(glm::vec3 pos, glm::vec3 look, glm::vec3 up);

    // Returns the aspect ratio of the camera.
    float getAspectRatio() const;

    // Returns the height angle of the camera in RADIANS.
    float getHeightAngle() const;

    // Returns the focal length of this camera.
    // This is for the depth of field extra-credit feature only;
    // You can ignore if you are not attempting to implement depth of field.
    float getFocalLength() const;

    // Returns the focal length of this camera.
    // This is for the depth of field extra-credit feature only;
    // You can ignore if you are not attempting to implement depth of field.
    float getAperture() const;

    // Returns the focal length of this camera.
    // This is for the depth of field extra-credit feature only;
    // You can ignore if you are not attempting to implement depth of field.
    glm::vec4 getPos() const;

    //calculate view matrices once and then store, don't recalculate each time
    glm::mat4 calcViewMat() const;

    glm::mat4 getProjMat() const;

    glm::mat4 calcProjMat();

    void updateProjMat(float near, float far);

    void updateCamera(glm::vec3 pos, glm::vec3 look, glm::vec3 up, float heightAng, float focalL,
                      float aperture, int width, int height, float near, float far);




private:
    //camera settings:
    glm::vec3 m_pos;
    glm::vec3 m_look;
    glm::vec3 m_up;
    float m_heightAng;
    float m_widthAng;
    float m_focalL;
    float m_aperture;
    int m_width;
    int m_height;
    glm::mat4 viewMat;
    glm::mat4 inverseViewMat;
    glm::mat4 projMat;

    float widthAng;
    float aspRat;
    float m_near;
    float m_far;

};

