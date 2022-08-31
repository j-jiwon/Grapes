#include "context.h"
#include "image.h"

std::unique_ptr<Context> Context::Create() {
    auto context = std::unique_ptr<Context>(new Context());
    if (!context->Init())
        return nullptr;
    return std::move(context);
}

void Context::Render() {
    glClear(GL_COLOR_BUFFER_BIT);
    program->Use();
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
}

bool Context::Init() {

    float vertices[] = {
        // x, y, z, r, g, b, u, v
        -0.5f, 0.5f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f, // LT - Y
        0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, // RT - R
        -0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, // LB - B
        0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f // RB - G
    };

    uint32_t indices[] = {
        0, 1, 2,
        1, 2, 3,
    };

    vertexLayout = VertexLayout::Create();
    vertexBuffer = Buffer::CreateWithData(GL_ARRAY_BUFFER, GL_STATIC_DRAW, vertices, sizeof(float) * 32);
    vertexLayout->SetAttrib(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 8, 0);
    vertexLayout->SetAttrib(1, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 8, sizeof(float) * 3);
    vertexLayout->SetAttrib(2, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 8, sizeof(float) * 6);

    indexBuffer = Buffer::CreateWithData(GL_ELEMENT_ARRAY_BUFFER, GL_STATIC_DRAW, indices, sizeof(uint32_t) * 6);

    std::shared_ptr<Shader> vertexShader = Shader::CreateFromFile("./shader/texture.vs", GL_VERTEX_SHADER);
    std::shared_ptr<Shader> fragmentShader = Shader::CreateFromFile("./shader/texture.fs", GL_FRAGMENT_SHADER);
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
    glUniform1i(glGetUniformLocation(program->Get(), "tex"), 0);
    glUniform1i(glGetUniformLocation(program->Get(), "tex2"), 1);
    
    // rotate 90 degree, z axis
    auto transform = glm::rotate(glm::mat4(1.0f), glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
    auto transformLoc = glGetUniformLocation(program->Get(), "transform");
    glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(transform));

    return true;
}
