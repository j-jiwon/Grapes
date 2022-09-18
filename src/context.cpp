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
            ImGui::DragFloat3("l.position", glm::value_ptr(light.position), 0.01f);
            ImGui::DragFloat3("l.direction", glm::value_ptr(light.direction), 0.01f);
            ImGui::DragFloat("l.cutoff", glm::value_ptr(light.cutoff), 0.5f, 0.0f, 90.0f);
            ImGui::DragFloat("l.distance", &light.distance, 0.5f, 0.0f, 3000.0f);
            ImGui::ColorEdit3("l.ambient", glm::value_ptr(light.ambient));
            ImGui::ColorEdit3("l.diffuse", glm::value_ptr(light.diffuse));
            ImGui::ColorEdit3("l.specular", glm::value_ptr(light.specular));
        }
        
        if (ImGui::CollapsingHeader("material", ImGuiTreeNodeFlags_DefaultOpen)) {
            ImGui::DragFloat("m.shininess", &material.shininess, 1.0f, 1.0f, 256.0f);
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
        (float)windowWidth / (float)windowHeight, 0.01f, 30.0f);

    auto view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);

    // after computing projection and view matrix
    auto lightModelTransform =
        glm::translate(glm::mat4(1.0), light.position) *
        glm::scale(glm::mat4(1.0), glm::vec3(0.1f));

    simpleProgram->Use();
    simpleProgram->SetUniform("color", glm::vec4(light.ambient + light.diffuse, 1.0f));
    simpleProgram->SetUniform("transform", projection * view * lightModelTransform);
    box->Draw();

    // set shader variables
    program->Use();
    program->SetUniform("viewPos", cameraPos);
	program->SetUniform("light.position", light.position);
    program->SetUniform("light.direction", light.direction);
	program->SetUniform("light.cutoff", glm::vec2(
        cosf(glm::radians(light.cutoff[0])),
        cosf(glm::radians(light.cutoff[0] + light.cutoff[1]))));
    program->SetUniform("light.attenuation", GetAttenuationCoeff(light.distance));
    program->SetUniform("light.ambient", light.ambient);
    program->SetUniform("light.diffuse", light.diffuse);
    program->SetUniform("light.specular", light.specular);
    program->SetUniform("material.diffuse", 0);
    program->SetUniform("material.specular", 1);
    program->SetUniform("material.shininess", material.shininess);

    glActiveTexture(GL_TEXTURE0);
    material.diffuse->Bind();
    glActiveTexture(GL_TEXTURE1);
    material.specular->Bind();

    for (size_t i = 0; i < cubePositions.size(); i++){
        auto& pos = cubePositions[i];
        auto model = glm::translate(glm::mat4(1.0f), pos);
        model = glm::rotate(model,
            glm::radians((animation ? (float)glfwGetTime() : 0.0f) * 120.0f + 20.0f * (float)i),
            glm::vec3(1.0f, 0.5f, 0.0f));
        auto transform = projection * view * model;
        program->SetUniform("transform", transform);
        program->SetUniform("modelTransform", model);
        box->Draw();
    }
}

bool Context::Init() {

    box = Mesh::CreateBox();

    simpleProgram = Program::Create("./shader/simple.vs", "./shader/simple.fs");
    if (!simpleProgram)
        return false;

    program = Program::Create("./shader/lighting.vs", "./shader/lighting.fs");
    if (!program)
        return false;

    glClearColor(clearColor.r, clearColor.g, clearColor.b, clearColor.a);
    glEnable(GL_DEPTH_TEST);

    material.diffuse = Texture::CreateFromImage(Image::Load("./images/container2.png").get());
    material.specular = Texture::CreateFromImage(Image::Load("./images/container2_specular.png").get());

    return true;
}
