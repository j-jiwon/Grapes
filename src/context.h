#ifndef __CONTEXT_H__
#define __CONTEXT_H__

#include "common.h"
#include "shader.h"
#include "program.h"
#include "buffer.h"
#include "vertex_layout.h"
#include "texture.h"
#include "mesh.h"
#include "model.h"

class Context {
public:
    static std::unique_ptr<Context> Create();
    void Render();    
    void ProcessInput(GLFWwindow* window);
    void MouseMove(double x, double y);
    void MouseButton(int button, int action, double x, double y);
    void Reshape(int width, int height);

private:
    Context() {}
    bool Init();
    std::unique_ptr<Program> program;
    std::unique_ptr<Program> simpleProgram;

    std::unique_ptr<Mesh> box;
    std::unique_ptr<Model> model;
    std::unique_ptr<Texture> textureId;
    std::unique_ptr<Texture> textureId2;

    bool animation { true };

    glm::vec4 clearColor { glm::vec4(0.1f, 0.2f, 0.3f, 0.0f) };

    struct Light {
	    glm::vec3 position { glm::vec3(1.0f, 1.0f, 1.0f) };
        glm::vec3 direction { glm::vec3(-1.0f, -1.0f, -1.0f) };
        glm::vec2 cutoff { glm::vec2(20.0f, 5.0f) };
        float distance { 32.0f };
        glm::vec3 ambient { glm::vec3(0.8f, 0.8f, 0.8f) };
        glm::vec3 diffuse { glm::vec3(0.5f, 0.5f, 0.5f) };
        glm::vec3 specular { glm::vec3(1.0f, 1.0f, 1.0f) };
    };
    Light light;
    
    std::shared_ptr<Material> material;

    bool cameraControl { false };
    glm::vec2 prevMousePos { glm::vec2(0.0f) };
    float cameraPitch { 0.0f };
    float cameraYaw { 0.0f };
    glm::vec3 cameraPos { glm::vec3(0.0f, 0.0f, 3.0f) };
    glm::vec3 cameraFront { glm::vec3(0.0f, -1.0f, 0.0f) } ;
    glm::vec3 cameraUp { glm::vec3(0.0f, 1.0f, 0.0f) };

    int windowWidth = {WINDOW_WIDTH};
    int windowHeight = {WINDOW_HEIGHT};
};

#endif
