#include "realtime.h"

#include <QCoreApplication>
#include <QMouseEvent>
#include <QKeyEvent>
#include <iostream>
#include <unordered_set>
#include "settings.h"
#include "shapes/Cone.h"
#include "utils/debug.h"
#include "utils/sceneparser.h"
#include "utils/shaderloader.h"
#include "shapes/Sphere.h"
#include "shapes/Cube.h"
#define TINYOBJLOADER_IMPLEMENTATION
#include <utils/tiny_obj_loader.h>


// ================== Project 5: Lights, Camera

Realtime::Realtime(QWidget *parent)
    : QOpenGLWidget(parent)
{
    m_prev_mouse_pos = glm::vec2(size().width()/2, size().height()/2);
    setMouseTracking(true);
    setFocusPolicy(Qt::StrongFocus);

    m_keyMap[Qt::Key_W]       = false;
    m_keyMap[Qt::Key_A]       = false;
    m_keyMap[Qt::Key_S]       = false;
    m_keyMap[Qt::Key_D]       = false;
    m_keyMap[Qt::Key_Control] = false;
    m_keyMap[Qt::Key_Space]   = false;

    // If you must use this function, do not edit anything above this
}

// Function to perform cleanup when the program exits
void Realtime::finish() {
    killTimer(m_timer);
    this->makeCurrent();

    // Students: Cleanup OpenGL resources here
    glDeleteBuffers(1, &sphere_vbo);
    glDeleteVertexArrays(1, &sphere_vao);
    glDeleteBuffers(1, &cone_vbo);
    glDeleteVertexArrays(1, &cone_vao);
    glDeleteBuffers(1, &cube_vbo);
    glDeleteVertexArrays(1, &cube_vao);
    glDeleteBuffers(1, &cyl_vbo);
    glDeleteVertexArrays(1, &cyl_vao);
    for(Model model : m_models) {
        glDeleteBuffers(1, &model.model_vbo);
        glDeleteVertexArrays(1, &model.model_vao);
    }

    //delete fbo stuff
    glDeleteTextures(1, &m_fbo_texture);
    glDeleteRenderbuffers(1, &m_fbo_renderbuffer);
    glDeleteVertexArrays(1, &m_fullscreen_vao);
    glDeleteBuffers(1, &m_fullscreen_vbo);
    glDeleteFramebuffers(1, &m_fbo);

    //delete shaders
    glDeleteProgram(m_shader);
    glDeleteProgram(m_texture_shader);


    this->doneCurrent();
}


void Realtime::initializeGL() {
    m_devicePixelRatio = this->devicePixelRatio();

    m_timer = startTimer(1000/60);
    m_elapsedTimer.start();

    // Initializing GL.
    // GLEW (GL Extension Wrangler) provides access to OpenGL functions.
    glewExperimental = GL_TRUE;
    GLenum err = glewInit();
    if (err != GLEW_OK) {
        std::cerr << "Error while initializing GL: " << glewGetErrorString(err) << std::endl;
    }
    std::cout << "Initialized GL: Version " << glewGetString(GLEW_VERSION) << std::endl;

    // Allows OpenGL to draw objects appropriately on top of one another
    glEnable(GL_DEPTH_TEST);
    // Tells OpenGL to only draw the front face
    glEnable(GL_CULL_FACE);
    // Tells OpenGL how big the screen is
    glViewport(0, 0, size().width() * m_devicePixelRatio, size().height() * m_devicePixelRatio);

    // Students: anything requiring OpenGL calls when the program starts should be done here
    //set up shader here
    m_shader = ShaderLoader::createShaderProgram(":/resources/shaders/default.vert", ":/resources/shaders/default.frag");
    m_texture_shader = ShaderLoader::createShaderProgram(":/resources/shaders/texture.vert", ":/resources/shaders/texture.frag");
    m_model_shader = ShaderLoader::createShaderProgram(":/resources/shaders/model.vert", ":/resources/shaders/model.frag");

    //store variables
    storeVariables();

    //create camera
    m_camera = Camera();

    //fill ctms
    fillCTMs();
    setUpTextures();

    std::vector<std::string> paths = std::vector<std::string> {
//        "/Users/andyburris/School/Semester 5/cs1230/diffuse-defenestration/scenefiles/demo-viking/viking_room.obj",
//        "/Users/andyburris/School/Semester 5/cs1230/diffuse-defenestration/scenefiles/demo-viking/viking_room.png",
        "/Users/andyburris/School/Semester 5/cs1230/diffuse-defenestration/scenefiles/plane/CanyonPlane.obj",
        "/Users/andyburris/School/Semester 5/cs1230/diffuse-defenestration/scenefiles/plane/BaseColor.png",
        "/Users/andyburris/School/Semester 5/cs1230/diffuse-defenestration/scenefiles/plane/CanyonLowPoly.obj",
        "/Users/andyburris/School/Semester 5/cs1230/diffuse-defenestration/scenefiles/plane/CanyonTerrain.png",
        "/Users/andyburris/School/Semester 5/cs1230/diffuse-defenestration/scenefiles/plane/Skybox.obj",
        "/Users/andyburris/School/Semester 5/cs1230/diffuse-defenestration/scenefiles/plane/Skybox.png"
    };
    for(int i = 0; i < paths.size(); i+=2) {
        std::string modelPath = paths[i];
        std::string texturePath = paths[i + 1];
        initializeModel(modelPath, texturePath, i / 2);
    }


    glUseProgram(0);

    update();
}

void Realtime::initializeModel(std::string modelPath, std::string texturePath, int index) {
    Model model;
    model.textureIndex = index + 1;

    // Load model data
    loadModel(model, modelPath, texturePath);

    //init texture
    initializeTexture(model);

    initializeModelBuffer(model);

    m_models.push_back(model);
}

void Realtime::initializeModelBuffer(Model &model) {
    glGenBuffers(1, &model.model_vbo);
    glBindBuffer(GL_ARRAY_BUFFER, model.model_vbo);
    // Send data to VBO
    glBufferData(GL_ARRAY_BUFFER, model.modelData.size() * sizeof(GLfloat), model.modelData.data(), GL_STATIC_DRAW);
    // Generate, and bind vao
    glGenVertexArrays(1, &model.model_vao);
    glBindVertexArray(model.model_vao);

    //  Enable and define attribute 1 to store vertex normals
    glEnableVertexAttribArray(0); //position
    glEnableVertexAttribArray(1); //normal
    glEnableVertexAttribArray(2); //tex coord
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat),
                          nullptr);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat),
                          reinterpret_cast<void *>(3 * sizeof(GLfloat)));
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat),
                          reinterpret_cast<void *>(6 * sizeof(GLfloat)));

    // Clean-up bindings
    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER,0);
}

void Realtime::initializeTexture(Model &model) {
    glGenTextures(1, &model.textureID);
    glActiveTexture(GL_TEXTURE0 + model.textureIndex); // Use GL_TEXTURE1 as the texture unit
    glBindTexture(GL_TEXTURE_2D, model.textureID);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, model.modelTexture.width(), model.modelTexture.height(), 0, GL_RGBA, GL_UNSIGNED_BYTE, model.modelTexture.bits());
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glBindTexture(GL_TEXTURE_2D, 0); // Unbind the texture
    // Note: Make sure to keep the shader program bound if you intend to set uniforms immediately after this
    glUseProgram(m_model_shader);

}

void Realtime::loadModel(Model &model, std::string modelPath, std::string texturePath) {

    tinyobj::ObjReaderConfig reader_config;
    reader_config.mtl_search_path = ""; // Path to material files

    model.modelTexture = QImage(texturePath.c_str()).convertToFormat(QImage::Format_RGBA8888).mirrored();

    tinyobj::ObjReader reader;

    if (!reader.ParseFromFile(modelPath, reader_config)) {
        if (!reader.Error().empty()) {
            std::cerr << "TinyObjReader: " << reader.Error();
        }
        exit(1);
    }

    if (!reader.Warning().empty()) {
        std::cout << "TinyObjReader: " << reader.Warning();
    }

    auto& attrib = reader.GetAttrib();
    auto& shapes = reader.GetShapes();
    auto& materials = reader.GetMaterials();

    // Loop over shapes
    for (size_t s = 0; s < shapes.size(); s++) {
        // Loop over faces(polygon)
        size_t index_offset = 0;
        for (size_t f = 0; f < shapes[s].mesh.num_face_vertices.size(); f++) {
            size_t fv = size_t(shapes[s].mesh.num_face_vertices[f]);

            // Loop over vertices in the face.
            for (size_t v = 0; v < fv; v++) {
                // access to vertex
                tinyobj::index_t idx = shapes[s].mesh.indices[index_offset + v];
                tinyobj::real_t vx = attrib.vertices[3*size_t(idx.vertex_index)+0];
                tinyobj::real_t vy = attrib.vertices[3*size_t(idx.vertex_index)+1];
                tinyobj::real_t vz = attrib.vertices[3*size_t(idx.vertex_index)+2];

                // Check if `normal_index` is zero or positive. negative = no normal data
                // Check if `texcoord_index` is zero or positive. negative = no texcoord data
                if (idx.normal_index >= 0 && idx.texcoord_index >= 0) {
                    tinyobj::real_t nx = attrib.normals[3*size_t(idx.normal_index)+0];
                    tinyobj::real_t ny = attrib.normals[3*size_t(idx.normal_index)+1];
                    tinyobj::real_t nz = attrib.normals[3*size_t(idx.normal_index)+2];
                    tinyobj::real_t tx = attrib.texcoords[2*size_t(idx.texcoord_index)+0];
                    tinyobj::real_t ty = attrib.texcoords[2*size_t(idx.texcoord_index)+1];

                    model.modelData.push_back(vx); model.modelData.push_back(vy); model.modelData.push_back(vz);
                    model.modelData.push_back(nx); model.modelData.push_back(ny); model.modelData.push_back(nz);
                    model.modelData.push_back(tx); model.modelData.push_back(ty);
                }
            }
            index_offset += fv;

            // per-face material
            shapes[s].mesh.material_ids[f];
        }
    }
}

void Realtime::paintModel(Model &model) {
//    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glBindVertexArray(model.model_vao);

    // clear
    // Task 24: Bind FBO
    glBindFramebuffer(GL_FRAMEBUFFER, m_fbo);    
    // Task 28: Call glViewport
    glViewport(0, 0,  m_screen_width, m_screen_height);
    //link shader
    glUseProgram(m_model_shader);

    glUniformMatrix4fv(glGetUniformLocation(m_model_shader, "viewMatrix"), 1, GL_FALSE, &m_view[0][0]);
    glUniformMatrix4fv(glGetUniformLocation(m_model_shader, "projMatrix"), 1, GL_FALSE, &m_proj[0][0]);

    if(m_renderData.shapes.size() > 0) { //TODO: figure out how to put this into the JSON
        lightsForShader(m_model_shader, m_renderData.shapes[0].primitive.material);
    }

    glActiveTexture(GL_TEXTURE0 + model.textureIndex);
    glBindTexture(GL_TEXTURE_2D, model.textureID);
    glUniform1i(glGetUniformLocation(m_model_shader, "textureSampler"), model.textureIndex); // Use texture unit GL_TEXTURE1

    // Draw Command
    glDrawArrays(GL_TRIANGLES, 0, model.modelData.size() / 8);

    // Unbind Vertex Array and texture
    glBindVertexArray(0);
    glBindTexture(GL_TEXTURE_2D, 0);

    //deactivate the shader program by passing 0 into glUseProgram
    glUseProgram(0);
    Debug::glErrorCheck();
}


void Realtime::genBufferData(){
    int i = 0;
    glm::mat4 currCTM;
    int param1 = settings.shapeParameter1;
    int param2 = settings.shapeParameter2;
    for (const RenderShapeData& shape : m_renderData.shapes) {
        std::unordered_set<PrimitiveType> processedShapes;
        glm::mat4 currCTM = ctm_list.at(i); //get ctm
        i+=1;
        if (processedShapes.find(shape.primitive.type) == processedShapes.end()) {
            // Mark the shape type as processed
            processedShapes.insert(shape.primitive.type);
            //switch statement for each shape
            switch(shape.primitive.type){
            case(PrimitiveType::PRIMITIVE_CUBE):
                glGenBuffers(1, &cube_vbo);
                glBindBuffer(GL_ARRAY_BUFFER, cube_vbo);
                // Generate vertex data
                m_cube = Cube();
                m_cube.updateParams(param1);
                m_cubeData = m_cube.generateShape();
                // Send data to VBO
                glBufferData(GL_ARRAY_BUFFER,m_cubeData.size() * sizeof(GLfloat),m_cubeData.data(), GL_STATIC_DRAW);
                // Generate, and bind vao
                glGenVertexArrays(1, &cube_vao);
                glBindVertexArray(cube_vao);
                break;
            case(PrimitiveType::PRIMITIVE_CONE):
                glGenBuffers(1, &cone_vbo);
                glBindBuffer(GL_ARRAY_BUFFER, cone_vbo);
                // Generate vertex data
                m_cone = Cone();
                m_cone.updateParams(param1, param2);
                m_coneData = m_cone.generateShape();
                // Send data to VBO
                glBufferData(GL_ARRAY_BUFFER,m_coneData.size() * sizeof(GLfloat),m_coneData.data(), GL_STATIC_DRAW);
                // Generate, and bind vao
                glGenVertexArrays(1, &cone_vao);
                glBindVertexArray(cone_vbo);
                break;
            case(PrimitiveType::PRIMITIVE_CYLINDER):
                glGenBuffers(1, &cyl_vbo);
                glBindBuffer(GL_ARRAY_BUFFER, cyl_vbo);
                // Generate vertex data
                m_cyl = Cylinder();
                m_cyl.updateParams(param1, param2);
                m_cylData = m_cyl.generateShape();
                // Send data to VBO
                glBufferData(GL_ARRAY_BUFFER,m_cylData.size() * sizeof(GLfloat),m_cylData.data(), GL_STATIC_DRAW);
                // Generate, and bind vao
                glGenVertexArrays(1, &cyl_vao);
                glBindVertexArray(cyl_vbo);
                break;
            case(PrimitiveType::PRIMITIVE_SPHERE):
                glGenBuffers(1, &sphere_vbo);
                glBindBuffer(GL_ARRAY_BUFFER, sphere_vbo);
                // Generate sphere data
                m_sphere = Sphere();
                m_sphere.updateParams(param1, param2);
                m_sphereData = m_sphere.generateShape();
                // Send data to VBO
                glBufferData(GL_ARRAY_BUFFER,m_sphereData.size() * sizeof(GLfloat),m_sphereData.data(), GL_STATIC_DRAW);
                // Generate, and bind vao
                glGenVertexArrays(1, &sphere_vao);
                glBindVertexArray(sphere_vao);
                break;
            default:
                break;
            }

            //  Enable and define attribute 1 to store vertex normals
            glEnableVertexAttribArray(0);
            glEnableVertexAttribArray(1);
            glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat),
                                  nullptr);
            glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat),
                                  reinterpret_cast<void *>(3 * sizeof(GLfloat)));

            // Clean-up bindings
            glBindVertexArray(0);
            glBindBuffer(GL_ARRAY_BUFFER,0);
        }
    }
}

void Realtime::lightsForShader(int shaderToFill, SceneMaterial material) {
    int lightCount = fmin(8, m_renderData.lights.size());
    glUniform1i(glGetUniformLocation(shaderToFill, "lightCount"), lightCount);

    //we don't actually need the different light types here but I though we did so I'm leaving it tho I don't actually use it
    std::string lightUniforms[] = {
        "lightIntensities",
        "lightDirs",
        "lightPositions",
        "lightTypes",
        "lightFunctions",
        "lightAngles",
        "lightPenumbras"
    };

    // fill light uniforms with light data for phong
    for (int j = 0; j < lightCount; j++) {
        SceneLightData curLight = m_renderData.lights[j];
        for (const std::string& uniform : lightUniforms) {
            std::string uniformStr = uniform + "[" + std::to_string(j) + "]";
            GLint uniformLoc = glGetUniformLocation(shaderToFill, uniformStr.c_str());

            if (uniform == "lightIntensities") {
                glUniform4fv(uniformLoc, 1, &curLight.color[0]);
            } else if (uniform == "lightDirs") {
                glUniform3f(uniformLoc, curLight.dir.x, curLight.dir.y, curLight.dir.z);
            } else if (uniform == "lightPositions") {
                glUniform3f(uniformLoc, curLight.pos.x, curLight.pos.y, curLight.pos.z);
            } else if (uniform == "lightFunctions") {
                glUniform3f(uniformLoc, curLight.function.x, curLight.function.y, curLight.function.z);
            } else if (uniform == "lightAngles") {
                glUniform1f(uniformLoc, curLight.angle);
            } else if (uniform == "lightPenumbras") {
                glUniform1f(uniformLoc, curLight.penumbra);
            } else if (uniform == "lightTypes") {
                //1 = directional, 2 = point, 3 = spot
                switch (curLight.type) {
                case LightType::LIGHT_DIRECTIONAL:
                    glUniform1i(uniformLoc, 1);
                    break;
                case LightType::LIGHT_POINT:
                    glUniform1i(uniformLoc, 2);
                    break;
                case LightType::LIGHT_SPOT:
                    glUniform1i(uniformLoc, 3);
                    break;
                }
            }
        }
    }

    //pass ka,kd,ks, shininess, and cam.pos into fragment shader
    glUniform1f(glGetUniformLocation(shaderToFill, "k_a"), m_ka);
    glUniform1f(glGetUniformLocation(shaderToFill, "k_d"), m_kd);
    glUniform1f(glGetUniformLocation(shaderToFill, "k_s"), m_ks);
    glm::vec4 cameraPos = m_viewInverse[3];
    glUniform4fv(glGetUniformLocation(shaderToFill, "worldCamPos"), 1, &cameraPos[0]);
    glUniform1f(glGetUniformLocation(shaderToFill, "shininess"), material.shininess);

    glUniform4fv(glGetUniformLocation(shaderToFill, "c_a"), 1, &material.cAmbient[0]);
    glUniform4fv(glGetUniformLocation(shaderToFill, "c_d"), 1, &material.cDiffuse[0]);
    glUniform4fv(glGetUniformLocation(shaderToFill, "c_s"), 1, &material.cSpecular[0]);

}


void Realtime::fillCTMs(){
    //also store ca etc. stuff and other shape specific things?
    ctm_list.clear();
    inverse_ctm_list.clear();
    for (const RenderShapeData& shape : m_renderData.shapes) {
        ctm_list.push_back(shape.ctm);
        inverse_ctm_list.push_back(glm::inverse(shape.ctm));
    }
}

void Realtime::storeVariables(){
    m_ka = m_renderData.globalData.ka;
    m_kd = m_renderData.globalData.kd;
    m_ks = m_renderData.globalData.ks;

    m_near = settings.nearPlane;
    m_far = settings.farPlane;
    m_param1 = settings.shapeParameter1;
    m_param2 = settings.shapeParameter2;
}

void Realtime::paintGeometry(){
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glm::mat4 currCTM;
    int i = 0;
    int count = 1; //used in draw command depending on the shape
    glm::vec4 ca;
    glm::vec4 cd;
    glm::vec4 cs;
    float shininess;

    for (const RenderShapeData& shape : m_renderData.shapes) {
        currCTM = ctm_list[i];
        //switch statement for each shape
        switch(shape.primitive.type){
        case(PrimitiveType::PRIMITIVE_CUBE):
            glBindVertexArray(cube_vao);
            count = m_cubeData.size()/6;
            break;
        case(PrimitiveType::PRIMITIVE_CONE):
            glBindVertexArray(cone_vao);
            count = m_coneData.size()/6;
            break;
        case(PrimitiveType::PRIMITIVE_CYLINDER):
            glBindVertexArray(cyl_vao);
            count = m_cylData.size()/6;
            break;
        case(PrimitiveType::PRIMITIVE_SPHERE):
            glBindVertexArray(sphere_vao);
            count = m_sphereData.size()/6;
            break;
        default:
            break;
        }
        ca = shape.primitive.material.cAmbient;
        cd = shape.primitive.material.cDiffuse;
        cs = shape.primitive.material.cReflective;
        shininess = shape.primitive.material.shininess;

        // clear
        // Task 24: Bind FBO
        glBindFramebuffer(GL_FRAMEBUFFER, m_fbo);
        // Task 28: Call glViewport
        glViewport(0, 0,  m_screen_width, m_screen_height);
        //link shader
        glUseProgram(m_shader);

        //pass in lights
        lightsForShader(m_shader, shape.primitive.material);

        //pass model matrix, view matrix, and projection matrix into the vertex shader
        glUniformMatrix4fv(glGetUniformLocation(m_shader, "modelMatrix"), 1, GL_FALSE, &currCTM[0][0]);
        glUniformMatrix4fv(glGetUniformLocation(m_shader, "viewMatrix"), 1, GL_FALSE, &m_view[0][0]);
        glUniformMatrix4fv(glGetUniformLocation(m_shader, "projMatrix"), 1, GL_FALSE, &m_proj[0][0]);

        // Draw Command
        glDrawArrays(GL_TRIANGLES, 0, count);

        // Unbind Vertex Array
        glBindVertexArray(0);

        //deactivate the shader program by passing 0 into glUseProgram
        glUseProgram(0);

        i+=1;

    }
}

void Realtime::paintGL() {
    // Students: anything requiring OpenGL calls every frame should be done here
    // Clear screen color and depth before painting
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glBindFramebuffer(GL_FRAMEBUFFER, m_fbo);
    glViewport(0, 0,  m_fbo_width, m_fbo_height);

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

//    paintGeometry();
    for(Model model : m_models) {
        paintModel(model);
    }

    //FBO stuff
    // Task 25: Bind the default framebuffer
    glBindFramebuffer(GL_FRAMEBUFFER, m_defaultFBO);
    // Task 26: Clear the color and depth buffers
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    // Task 27: Call paintTexture to draw our FBO color attachment texture | Task 31: Set bool parameter to true
    paintTexture(m_fbo_texture, settings.perPixelFilter, settings.kernelBasedFilter, settings.extraCredit1, settings.extraCredit2, settings.extraCredit3);
}

void Realtime::setUpTextures(){
    // Create FBO
    //  // Task 10: Set the texture.frag uniform for our texture
    glUseProgram(m_texture_shader);
    GLint textureUniformLoc = glGetUniformLocation(m_texture_shader, "textureUniform1");
    glUniform1i(textureUniformLoc, 0);

    m_defaultFBO = 2;
    m_fbo_width = size().width() * m_devicePixelRatio;
    m_fbo_height = size().height() * m_devicePixelRatio;

    std::vector<GLfloat> fullscreen_quad_data =
        { //     POSITIONS    //
            -1.0f,  1.0f, 0.0f, // Top-left
            0.0f, 1.0f, // Top-left

            -1.0f, -1.0f, 0.0f, // Bottom-left
            0.0f, 0.0f, // Bottom-left

            1.0f, -1.0f, 0.0f, // Bottom-right
            1.0f, 0.0f, // Bottom-right

            1.0f,  1.0f, 0.0f, // Top-right
            1.0f, 1.0f, // Top-right

            -1.0f,  1.0f, 0.0f, // Top-left
            0.0f, 1.0f, // Top-left

            1.0f, -1.0f, 0.0f, // Bottom-right
            1.0f, 0.0f, // Bottom-right
        };

    // Generate and bind a VBO and a VAO for a fullscreen quad
    glGenBuffers(1, &m_fullscreen_vbo);
    glBindBuffer(GL_ARRAY_BUFFER, m_fullscreen_vbo);
    glBufferData(GL_ARRAY_BUFFER, fullscreen_quad_data.size()*sizeof(GLfloat), fullscreen_quad_data.data(), GL_STATIC_DRAW);
    glGenVertexArrays(1, &m_fullscreen_vao);
    glBindVertexArray(m_fullscreen_vao);

    // Task 14: modify the code below to add a second attribute to the vertex attribute array
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), nullptr);

    //add uv coord attribute
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), reinterpret_cast<void*>(3 * sizeof(GLfloat)));

    // Unbind the fullscreen quad's VBO and VAO
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    makeFBO();
}

void Realtime::makeFBO(){
    // Task 19: Generate and bind an empty texture, set its min/mag filter interpolation, then unbind
    glGenTextures(1, &m_fbo_texture);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, m_fbo_texture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, m_fbo_width, m_fbo_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glBindTexture(GL_TEXTURE_2D, 0);

    // Task 20: Generate and bind a renderbuffer of the right size, set its format, then unbind
    glGenRenderbuffers(1, &m_fbo_renderbuffer);
    glBindRenderbuffer(GL_RENDERBUFFER, m_fbo_renderbuffer);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, m_fbo_width, m_fbo_height);
    glBindRenderbuffer(GL_RENDERBUFFER, 0);

    // Task 18: Generate and bind an FBO
    glGenFramebuffers(1, &m_fbo);
    glBindFramebuffer(GL_FRAMEBUFFER, m_fbo);

    // Task 21: Add our texture as a color attachment, and our renderbuffer as a depth+stencil attachment, to our FBO
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_fbo_texture, 0);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, m_fbo_renderbuffer);

    // Task 22: Unbind the FBO
    glBindFramebuffer(GL_FRAMEBUFFER, m_defaultFBO);

}


void Realtime::paintTexture(GLuint texture, bool invert, bool blur, bool grey, bool sobel, bool sharpen){
    // Task 32: Set your bool uniform on whether or not to filter the texture drawn
    glUseProgram(m_texture_shader);
    GLint invertUniformLoc = glGetUniformLocation(m_texture_shader, "invert");
    glUniform1i(invertUniformLoc, invert);

    GLint blurUniformLoc = glGetUniformLocation(m_texture_shader, "blur");
    glUniform1i(blurUniformLoc, blur);

    GLint greyUniformLoc = glGetUniformLocation(m_texture_shader, "grey");
    glUniform1i(greyUniformLoc, grey);

    GLint sobUniformLoc = glGetUniformLocation(m_texture_shader, "sobel");
    glUniform1i(sobUniformLoc, sobel);

    GLint sharpUniformLoc = glGetUniformLocation(m_texture_shader, "sharpen");
    glUniform1i(sharpUniformLoc, sharpen);

    glUniform1i(glGetUniformLocation(m_texture_shader, "fbo_width"), m_fbo_width);
    glUniform1i(glGetUniformLocation(m_texture_shader, "fbo_height"), m_fbo_height);


    glBindVertexArray(m_fullscreen_vao);
//    glBindBuffer(GL_ARRAY_BUFFER, m_fullscreen_vbo);

    // Task 10: Bind "texture" to slot 0
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture);

    glDrawArrays(GL_TRIANGLES, 0, 6); //bug here?
    glBindTexture(GL_TEXTURE_2D, 0);
    glBindVertexArray(0);
    glUseProgram(0);
}

void Realtime::resizeGL(int w, int h) {
    // Tells OpenGL how big the screen is
    glViewport(0, 0, size().width() * m_devicePixelRatio, size().height() * m_devicePixelRatio);
    //this updates the projection matrix
    // Students: anything requiring OpenGL calls when the program starts should be done here
    m_height = h;
    m_width = w;

    //Delete Texture, Renderbuffer, and Framebuffer memory
    glDeleteTextures(1, &m_fbo_texture);
    glDeleteRenderbuffers(1, &m_fbo_renderbuffer);
    glDeleteFramebuffers(1, &m_fbo);

    m_screen_width = size().width() * m_devicePixelRatio;
    m_screen_height = size().height() * m_devicePixelRatio;

    m_fbo_width = m_screen_width;
    m_fbo_height = m_screen_height;

    // Regenerate your FBOs
    makeFBO();
}

void Realtime::sceneChanged() {
    //clear lights
    m_renderData.lights.clear();

    //clear/delete buffers and shader
    glDeleteBuffers(1, &sphere_vbo);
    glDeleteVertexArrays(1, &sphere_vao);
    glDeleteBuffers(1, &cone_vbo);
    glDeleteVertexArrays(1, &cone_vao);
    glDeleteBuffers(1, &cube_vbo);
    glDeleteVertexArrays(1, &cube_vao);
    glDeleteBuffers(1, &cyl_vbo);
    glDeleteVertexArrays(1, &cyl_vao);

    //parse file
    bool success = SceneParser::parse(settings.sceneFilePath, m_renderData);
    if (!success) {
        std::cerr << "Error: Unable to parse file" << std::endl;
        return;
    }

    //store variables
    fillCTMs();
    storeVariables();

    //update camera
    m_camera.updateCamera(m_renderData.cameraData.pos,
                      m_renderData.cameraData.look,
                      m_renderData.cameraData.up,
                      m_renderData.cameraData.heightAngle,
                      m_renderData.cameraData.focalLength,
                      m_renderData.cameraData.aperture,
                      m_width,
                      m_height, m_near, m_far);
    //these never change
    m_view = m_camera.getViewMatrix();
    m_viewInverse = m_camera.getViewMatrixInverse();

    //proj changes with camera updates
    m_proj = m_camera.getProjMat();

    //this changes when params update
    genBufferData();

    update(); // asks for a PaintGL() call to occur
}


void Realtime::settingsChanged() {
    //if m_param1 or 2 changes then recalc genBufferData
    if (m_param1 != settings.shapeParameter1 | m_param2 != settings.shapeParameter2){
        //update params
        m_param1 = settings.shapeParameter1;
        m_param2 = settings.shapeParameter2;

        //recalc vbos
        genBufferData();
    }
    //if near or far changes then update camera
    if (m_near != settings.nearPlane | m_far != settings.farPlane){
        m_near = settings.nearPlane;
        m_far = settings.farPlane;

        //reupdate camera
        m_camera.updateProjMat(m_near, m_far);
        m_proj = m_camera.getProjMat();
    }
    update(); // asks for a PaintGL() call to occur
}

// ================== Project 6: Action!

void Realtime::keyPressEvent(QKeyEvent *event) {
    m_keyMap[Qt::Key(event->key())] = true;
}

void Realtime::keyReleaseEvent(QKeyEvent *event) {
    m_keyMap[Qt::Key(event->key())] = false;
}

void Realtime::mousePressEvent(QMouseEvent *event) {
    if (event->buttons().testFlag(Qt::LeftButton)) {
        m_mouseDown = true;
        m_prev_mouse_pos = glm::vec2(event->position().x(), event->position().y());
    }
}

void Realtime::mouseReleaseEvent(QMouseEvent *event) {
    if (!event->buttons().testFlag(Qt::LeftButton)) {
        m_mouseDown = false;
    }
}


//use the Rodrigues formula to calculate the rotational matrix about a normalized axis u
glm::mat3 calculateRotMat(float theta, glm::vec3 u) {
    //rotate around camera access not
    return glm::mat3(cos(theta) + u.x*u.x*(1 - cos(theta)), u.x*u.y*(1 - cos(theta)) - u.z*sin(theta), u.x*u.z*(1 - cos(theta)) + u.y*sin(theta),
                     u.x*u.y*(1 - cos(theta)) + u.z*sin(theta), cos(theta) + u.y*u.y*(1-cos(theta)), u.y*u.z*(1-cos(theta)) - u.x*sin(theta),
                     u.x*u.z*(1 - cos(theta)) - u.y*sin(theta), u.y*u.z*(1 - cos(theta)) + u.x*sin(theta), cos(theta) + u.z*u.z*(1 - cos(theta)));
}


void Realtime::mouseMoveEvent(QMouseEvent *event) {
    if (m_mouseDown) {
        int posX = event->position().x();
        int posY = event->position().y();
        int deltaX = posX - m_prev_mouse_pos.x;
        int deltaY = posY - m_prev_mouse_pos.y;
        m_prev_mouse_pos = glm::vec2(posX, posY);

        // Use deltaX and deltaY to rotate the camera

        // x rotation (around the up axis) (yaw rotation)
        glm::mat3 xRot = calculateRotMat(5 * (float)deltaX / width(), glm::vec3(0, 1, 0));
        // Apply the rotation to the camera's position, look, and up vectors
        m_renderData.cameraData.pos = glm::vec4(xRot * glm::vec3(m_renderData.cameraData.pos), 1);
        m_renderData.cameraData.look = glm::vec4(xRot * glm::vec3(m_renderData.cameraData.look), 0);
        m_renderData.cameraData.up = glm::vec4(xRot * glm::vec3(m_renderData.cameraData.up), 0);

        // Calculate the vertical axis after the x rotation
        glm::vec3 vertAxis = normalize(cross(glm::vec3(m_renderData.cameraData.look), glm::vec3(m_renderData.cameraData.up)));

        // y rotation (around the vertical axis) (pitch)
        glm::mat3 yRot = calculateRotMat(5 * (float)deltaY / height(), vertAxis);
        // Apply the rotation to the camera's position, look, and up vectors
        m_renderData.cameraData.pos = glm::vec4(yRot * glm::vec3(m_renderData.cameraData.pos), 1);
        m_renderData.cameraData.look = glm::vec4(yRot * glm::vec3(m_renderData.cameraData.look), 0);
        m_renderData.cameraData.up = glm::vec4(yRot * glm::vec3(m_renderData.cameraData.up), 0);

        // Update the view matrix based on the new camera orientation
        m_view = m_camera.updateViewMat(m_renderData.cameraData.pos, m_renderData.cameraData.look, m_renderData.cameraData.up);

        // Calculate the inverse of the view matrix
        m_viewInverse = glm::inverse(m_view);

        update(); // asks for a PaintGL() call to occur
    }
}

void Realtime::timerEvent(QTimerEvent *event) {
    int elapsedms   = m_elapsedTimer.elapsed();
    float deltaTime = elapsedms * 0.001f;
    m_elapsedTimer.restart();

    // Use deltaTime and m_keyMap here to move around

    // move left
    if (m_keyMap[Qt::Key_A]) {
        //calculate lateral vector
        glm::vec4 lateral = glm::vec4(glm::cross(glm::vec3(m_renderData.cameraData.look), glm::vec3(m_renderData.cameraData.up)), 0);
        //scale by 5
        m_renderData.cameraData.pos -= 5*deltaTime*normalize(lateral);
    }

    // move right
    if (m_keyMap[Qt::Key_D]) {
        glm::vec4 lateral = glm::vec4(glm::cross(glm::vec3(m_renderData.cameraData.look), glm::vec3(m_renderData.cameraData.up)), 0);
        m_renderData.cameraData.pos += 5*deltaTime*normalize(lateral);
    }

    // move up
    if (m_keyMap[Qt::Key_Space]) {
        m_renderData.cameraData.pos += 5*deltaTime*glm::vec4(0,1,0,0);
    }

    // move down
    if (m_keyMap[Qt::Key_Control]) {
        m_renderData.cameraData.pos += 5*deltaTime*glm::vec4(0,-1,0,0);
    }

    // move forwards
    if (m_keyMap[Qt::Key_W]) {
        m_renderData.cameraData.pos += 5*deltaTime*normalize(m_renderData.cameraData.look);
    }
    // more backwards
    if (m_keyMap[Qt::Key_S]) {
        m_renderData.cameraData.pos -= 5*deltaTime*normalize(m_renderData.cameraData.look);
    }

    //even tho only position is changing
    m_view = m_camera.updateViewMat(m_renderData.cameraData.pos, m_renderData.cameraData.look, m_renderData.cameraData.up);
    m_viewInverse = glm::inverse(m_view);

    update(); // asks for a PaintGL() call to occur
}

// DO NOT EDIT
void Realtime::saveViewportImage(std::string filePath) {
    // Make sure we have the right context and everything has been drawn
    makeCurrent();

    int fixedWidth = 1024;
    int fixedHeight = 768;

    // Create Frame Buffer
    GLuint fbo;
    glGenFramebuffers(1, &fbo);
    glBindFramebuffer(GL_FRAMEBUFFER, fbo);

    // Create a color attachment texture
    GLuint texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, fixedWidth, fixedHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texture, 0);

    // Optional: Create a depth buffer if your rendering uses depth testing
    GLuint rbo;
    glGenRenderbuffers(1, &rbo);
    glBindRenderbuffer(GL_RENDERBUFFER, rbo);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, fixedWidth, fixedHeight);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, rbo);

    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
        std::cerr << "Error: Framebuffer is not complete!" << std::endl;
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        return;
    }

    // Render to the FBO
    glBindFramebuffer(GL_FRAMEBUFFER, fbo);
    glViewport(0, 0, fixedWidth, fixedHeight);

    // Clear and render your scene here
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    paintGL();

    // Read pixels from framebuffer
    std::vector<unsigned char> pixels(fixedWidth * fixedHeight * 3);
    glReadPixels(0, 0, fixedWidth, fixedHeight, GL_RGB, GL_UNSIGNED_BYTE, pixels.data());

    // Unbind the framebuffer to return to default rendering to the screen
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    // Convert to QImage
    QImage image(pixels.data(), fixedWidth, fixedHeight, QImage::Format_RGB888);
    QImage flippedImage = image.mirrored(); // Flip the image vertically

    // Save to file using Qt
    QString qFilePath = QString::fromStdString(filePath);
    if (!flippedImage.save(qFilePath)) {
        std::cerr << "Failed to save image to " << filePath << std::endl;
    }

    // Clean up
    glDeleteTextures(1, &texture);
    glDeleteRenderbuffers(1, &rbo);
    glDeleteFramebuffers(1, &fbo);
}
