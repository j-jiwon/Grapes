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
    /*
    auto image = Image::Load("./images/wall.jpg");
    if (!image)
        return false;
    SPDLOG_INFO("image: {} * {}, {} channels", image->GetWidth(), image->GetHeight(), image->GetChannelCount());

    textureId = Texture::CreateFromImage(image.get());
    */
   
    auto image = Image::Create(512, 512);
    image->SetCheckerBoardImage(16, 16);
    textureId = Texture::CreateFromImage(image.get());

    return true;
}
