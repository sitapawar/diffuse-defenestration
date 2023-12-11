#pragma once

// Defined before including GLEW to suppress deprecation messages on macOS
#include "camera/camera.h"
#include "utils/sceneparser.h"
#ifdef __APPLE__
#define GL_SILENCE_DEPRECATION
#endif
#include <GL/glew.h>
#include <glm/glm.hpp>
#include "shapes/Sphere.h"
#include "shapes/Cube.h"
#include "shapes/Cone.h"
#include "shapes/Cylinder.h"
#include <unordered_map>
#include <QElapsedTimer>
#include <QOpenGLWidget>
#include <QTime>
#include <QTimer>

class Realtime : public QOpenGLWidget
{
public:
    Realtime(QWidget *parent = nullptr);
    void finish();                                      // Called on program exit
    void sceneChanged();
    void settingsChanged();
    void saveViewportImage(std::string filePath);
    void genBufferData();
    void fillCTMs();
    void storeVariables();
    void lightsForShader(int shaderToFill, SceneMaterial material);
    void setUpTextures();
    void paintGeometry();
    void paintTexture(GLuint texture, bool invert, bool blur, bool grey, bool sobel, bool sharpen);
    void makeFBO();

    void initializeModelBuffer();
    void loadModel();
    void paintModel();

public slots:
    void tick(QTimerEvent* event);                      // Called once per tick of m_timer

protected:
    void initializeGL() override;                       // Called once at the start of the program
    void paintGL() override;                            // Called whenever the OpenGL context changes or by an update() request
    void resizeGL(int width, int height) override;      // Called when window size changes

private:
    void keyPressEvent(QKeyEvent *event) override;
    void keyReleaseEvent(QKeyEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void timerEvent(QTimerEvent *event) override;

    GLuint m_fbo_texture;
    GLuint m_fbo;
    GLuint m_fbo_renderbuffer;
    GLuint m_defaultFBO;
    int m_fbo_width;
    int m_fbo_height;
    int m_screen_width;
    int m_screen_height;

    GLuint m_fullscreen_vbo;
    GLuint m_fullscreen_vao;


    // Tick Related Variables
    int m_timer;                                        // Stores timer which attempts to run ~60 times per second
    QElapsedTimer m_elapsedTimer;                       // Stores timer which keeps track of actual time between frames

    // Input Related Variables
    bool m_mouseDown = false;                           // Stores state of left mouse button
    glm::vec2 m_prev_mouse_pos;                         // Stores mouse position
    std::unordered_map<Qt::Key, bool> m_keyMap;         // Stores whether keys are pressed or not

    // Device Correction Variables
    int m_devicePixelRatio;

    //Realtime Pipeline Variables
    RenderData m_renderData;
    GLuint m_shader;
    GLuint m_texture_shader;

    GLuint m_model_shader;

    //vbos and vaos
    GLuint cube_vbo;
    GLuint cube_vao;
    GLuint sphere_vbo;
    GLuint sphere_vao;
    GLuint cone_vbo;
    GLuint cone_vao;
    GLuint cyl_vbo;
    GLuint cyl_vao;

    GLuint model_vbo;
    GLuint model_vao;

    //vert data
    std::vector<float> m_sphereData;
    std::vector<float> m_cubeData;
    std::vector<float> m_coneData;
    std::vector<float> m_cylData;

    std::vector<float> m_modelData;

    //shape obj
    Cube m_cube;
    Sphere m_sphere;
    Cone m_cone;
    Cylinder m_cyl;

    std::vector<glm::mat4> ctm_list;
    std::vector<glm::mat4> inverse_ctm_list;

    glm::mat4 m_view;
    glm::mat4 m_viewInverse;
    glm::mat4 m_proj;
    glm::vec4 m_lightPos; // The world-space position of the point light

    float m_ka;
    float m_kd;
    float m_ks;

    float m_param1;
    float m_param2;
    float m_near;
    float m_far;
    int m_height;
    int m_width;

    Camera m_camera;

};
