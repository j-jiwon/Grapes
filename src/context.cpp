#include "context.h"
#include "image.h"
#include <imgui.h>

std::unique_ptr<Context> Context::Create() {
    auto context = std::unique_ptr<Context>(new Context());
    if (!context->Init())
        return nullptr;
    return std::move(context);
}

void Context::ProcessInput(GLFWwindow* window){
    if (!cameraControl) return;

    const float cameraSpeed = 0.05f;
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        cameraPos += cameraSpeed * cameraFront;
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        cameraPos -= cameraSpeed * cameraFront;

    auto cameraRight = glm::normalize(glm::cross(cameraUp, -cameraFront));
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        cameraPos += cameraSpeed * cameraRight;
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        cameraPos -= cameraSpeed * cameraRight;

    auto cameraUp = glm::normalize(glm::cross(-cameraFront, cameraRight));
    if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
        cameraPos += cameraSpeed * cameraUp;
    if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
        cameraPos -= cameraSpeed * cameraUp;
}

void Context::Reshape(int width, int height){
    windowWidth = width;
    windowHeight = height;
    glViewport(0, 0, windowWidth, windowHeight);
}

void Context::MouseMove(double x, double y){
    if (!cameraControl) return;
    auto pos = glm::vec2((float)x, (float)y);
    auto deltaPos = pos - prevMousePos;

    const float cameraRotSpeed = 0.8f;
    cameraYaw -= deltaPos.x * cameraRotSpeed;
    cameraPitch -= deltaPos.y * cameraRotSpeed;

    if (cameraYaw < 0.0f) cameraYaw += 360.0f;
    if (cameraYaw > 360.0f) cameraYaw -= 360.0f;

    if (cameraPitch > 89.0f) cameraPitch = 89.0f;
    if (cameraPitch < -89.0f) cameraPitch = -89.0f;

    prevMousePos = pos;
}

void Context::MouseButton(int button, int action, double x, double y){
    if (button == GLFW_MOUSE_BUTTON_RIGHT) {
        if (action == GLFW_PRESS) {
            prevMousePos = glm::vec2((float)x, (float)y);
            cameraControl = true;
        }
        else if (action == GLFW_RELEASE) {
            cameraControl = false;
        }
    }
}


void Context::Render() {

    if(ImGui::Begin("ui window")) {
        if (ImGui::ColorEdit4("clear color", glm::value_ptr(clearColor)))
            glClearColor(clearColor.x, clearColor.y, clearColor.z, clearColor.w);
        ImGui::Separator();
        ImGui::DragFloat3("camera pos", glm::value_ptr(cameraPos), 0.01f);
        ImGui::DragFloat("camera yaw", &cameraYaw, 0.5f);
        ImGui::DragFloat("camera pitch", &cameraPitch, 0.5f, -89.0f, 89.0f);
        ImGui::Separator();
        if(ImGui::Button("reset camera")) {
            cameraYaw = 0.0f;
            cameraPitch = 0.0f;
            cameraPos = glm::vec3(0.0f, 0.0f, 3.0f);
        }

        if (ImGui::CollapsingHeader("light", ImGuiTreeNodeFlags_DefaultOpen)) {
            ImGui::DragFloat3("light pos", glm::value_ptr(lightPos), 0.01f);
            ImGui::ColorEdit3("light color", glm::value_ptr(lightColor));
            ImGui::ColorEdit3("object color", glm::value_ptr(objectColor));
            ImGui::SliderFloat("ambient strength", &ambientStrength, 0.0f, 1.0f);
        }

        ImGui::Checkbox("animation", &animation);
    }
    ImGui::End();
    
    std::vector<glm::vec3> cubePositions = {
        glm::vec3( 0.0f, 0.0f, 0.0f),
        glm::vec3( 2.0f, 5.0f, -15.0f),
        glm::vec3(-1.5f, -2.2f, -2.5f),
        glm::vec3(-3.8f, -2.0f, -12.3f),
        glm::vec3( 2.4f, -0.4f, -3.5f),
        glm::vec3(-1.7f, 3.0f, -7.5f),
        glm::vec3( 1.3f, -2.0f, -2.5f),
        glm::vec3( 1.5f, 2.0f, -2.5f),
        glm::vec3( 1.5f, 0.2f, -1.5f),
        glm::vec3(-1.3f, 1.0f, -1.5f),
    };

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glEnable(GL_DEPTH_TEST);

    cameraFront = glm::rotate(glm::mat4(1.0f), glm::radians(cameraYaw), glm::vec3(0.0f, 1.0f, 0.0f))
            * glm::rotate(glm::mat4(1.0f), glm::radians(cameraPitch), glm::vec3(1.0f, 0.0f, 0.0f))
            * glm::vec4(0.0f, 0.0f, -1.0f, 0.0f);


    auto projection = glm::perspective(glm::radians(45.0f),
        (float)windowWidth / (float)windowHeight, 0.01f, 60.0f);

    auto view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);

    // after computing projection and view matrix
    auto lightModelTransform =
    glm::translate(glm::mat4(1.0), lightPos) *
    glm::scale(glm::mat4(1.0), glm::vec3(0.1f));
    program->Use();
    program->SetUniform("lightPos", lightPos);
    program->SetUniform("lightColor", glm::vec3(1.0f, 1.0f, 1.0f));
    program->SetUniform("objectColor", glm::vec3(1.0f, 1.0f, 1.0f));
    program->SetUniform("ambientStrength", 1.0f);
    program->SetUniform("transform", projection * view * lightModelTransform);
    program->SetUniform("modelTransform", lightModelTransform);
    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

        // set shader variables
    program->Use();
    program->SetUniform("lightPos", lightPos);
    program->SetUniform("lightColor", lightColor);
    program->SetUniform("objectColor", objectColor);
    program->SetUniform("ambientStrength", ambientStrength);

    for (size_t i = 0; i < cubePositions.size(); i++){
        auto& pos = cubePositions[i];
        auto model = glm::translate(glm::mat4(1.0f), pos);
        model = glm::rotate(model,
            glm::radians((animation ? (float)glfwGetTime() : 0.0f) * 120.0f + 20.0f * (float)i),
            glm::vec3(1.0f, 0.5f, 0.0f));
        auto transform = projection * view * model;
        program->SetUniform("transform", transform);
        program->SetUniform("modelTransform", model);
        glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
    }
}

bool Context::Init() {

    float vertices[] = { // pos.xyz, normal.xyz, texcoord.uv
        -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 0.0f, 0.0f,
        0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 1.0f, 0.0f,
        0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 1.0f, 1.0f,
        -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 0.0f, 1.0f,

        -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f, 0.0f, 0.0f,
        0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f, 1.0f, 0.0f,
        0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f, 1.0f, 1.0f,
        -0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f, 0.0f, 1.0f,

        -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f, 1.0f, 0.0f,
        -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f, 1.0f, 1.0f,
        -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f, 0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f, 0.0f, 0.0f,

        0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f, 1.0f, 0.0f,
        0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f, 1.0f, 1.0f,
        0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f, 0.0f, 1.0f,
        0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f, 0.0f, 0.0f,

        -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f, 0.0f, 1.0f,
        0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f, 1.0f, 1.0f,
        0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f, 1.0f, 0.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f, 0.0f, 0.0f,

        -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f, 0.0f, 1.0f,
        0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f, 1.0f, 1.0f,
        0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f, 1.0f, 0.0f,
        -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f, 0.0f, 0.0f,
    };

    uint32_t indices[] = {
        0,  2,  1,  2,  0,  3,
        4,  5,  6,  6,  7,  4,
        8,  9, 10, 10, 11,  8,
        12, 14, 13, 14, 12, 15,
        16, 17, 18, 18, 19, 16,
        20, 22, 21, 22, 20, 23,
    };

    vertexLayout = VertexLayout::Create();
    vertexBuffer = Buffer::CreateWithData(GL_ARRAY_BUFFER, GL_STATIC_DRAW, vertices, sizeof(float) * 8 * 6 * 4);
    
    vertexLayout->SetAttrib(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 8, 0);
    vertexLayout->SetAttrib(1, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 8, sizeof(float) * 3);
    vertexLayout->SetAttrib(2, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 8, sizeof(float) * 6);
    
    indexBuffer = Buffer::CreateWithData(GL_ELEMENT_ARRAY_BUFFER, GL_STATIC_DRAW, indices, sizeof(uint32_t) * 36);

    std::shared_ptr<Shader> vertexShader = Shader::CreateFromFile("./shader/lighting.vs", GL_VERTEX_SHADER);
    std::shared_ptr<Shader> fragmentShader = Shader::CreateFromFile("./shader/lighting.fs", GL_FRAGMENT_SHADER);
    if (!vertexShader || !fragmentShader)
        return false;
    
    SPDLOG_INFO("vertex shader id: {}", vertexShader->Get());
    SPDLOG_INFO("fragment shader id: {}", fragmentShader->Get());

    program = Program::Create({fragmentShader, vertexShader});
    if (!program)
        return false;
    SPDLOG_INFO("program id: {}", program->Get());

    glClearColor(0.0f, 0.1f, 0.2f, 0.0f);

    // load image 
    auto image = Image::Load("./images/wall.jpg");
    if (!image)
        return false;
    SPDLOG_INFO("image: {} * {}, {} channels", image->GetWidth(), image->GetHeight(), image->GetChannelCount());

    textureId = Texture::CreateFromImage(image.get());
    
    auto image2 = Image::Load("./images/awesomeface.png");
    textureId2 = Texture::CreateFromImage(image2.get());
    glActiveTexture(GL_TEXTURE0);  // get number of texture slot -- slot0
    glBindTexture(GL_TEXTURE_2D, textureId->Get()); // bind textureId to slot0
    glActiveTexture(GL_TEXTURE1);  // slot1
    glBindTexture(GL_TEXTURE_2D, textureId2->Get());  // bind textureId2 to slot1

    program->Use();
    // notify program of the texture slot number to be used 
    program->SetUniform("tex", 0);
    program->SetUniform("tex2", 1);

    return true;
}
