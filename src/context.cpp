#include "context.h"

std::unique_ptr<Context> Context::Create() {
    auto context = std::unique_ptr<Context>(new Context());
    if (!context->Init())
        return nullptr;
    return std::move(context);
}

void Context::Render() {
    glClear(GL_COLOR_BUFFER_BIT);
    
    static float time = 0.0f;
    float t = sinf(time) * 0.5f + 0.5f; // 0 ~ 1
    auto loc = glGetUniformLocation(program->Get(), "color");
    program->Use();
    glUniform4f(loc, t*t, 2.0f*t*(1.0f-t), (1.0f-t)*(1.0f-t), 1.0f);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

    time += 0.0016f;
}

bool Context::Init() {

    float vertices[] = {
        -0.5f, 0.5f, 0.0f, // LT
        0.5f, 0.5f, 0.0f, // RT
        -0.5f, -0.5f, 0.0f, // LB
        0.5f, -0.5f, 0.0f,  // RB
    };

    uint32_t indices[] = {
        0, 1, 2,
        1, 2, 3,
    };

    vertexLayout = VertexLayout::Create();
    vertexBuffer = Buffer::CreateWithData(GL_ARRAY_BUFFER, GL_STATIC_DRAW, vertices, sizeof(float) * 12);
    vertexLayout->SetAttrib(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, 0);

    indexBuffer = Buffer::CreateWithData(GL_ELEMENT_ARRAY_BUFFER, GL_STATIC_DRAW, indices, sizeof(uint32_t) * 6);

    std::shared_ptr<Shader> vertexShader = Shader::CreateFromFile("./shader/simple.vs", GL_VERTEX_SHADER);
    std::shared_ptr<Shader> fragmentShader = Shader::CreateFromFile("./shader/simple.fs", GL_FRAGMENT_SHADER);
    if (!vertexShader || !fragmentShader)
        return false;
    
    SPDLOG_INFO("vertex shader id: {}", vertexShader->Get());
    SPDLOG_INFO("fragment shader id: {}", fragmentShader->Get());

    program = Program::Create({fragmentShader, vertexShader});
    if (!program)
        return false;
    SPDLOG_INFO("program id: {}", program->Get());

    // auto loc = glGetUniformLocation(program->Get(), "color");
    // program->Use();
    // glUniform4f(loc, 1.0f, 1.0f, 0.0f, 1.0f);

    glClearColor(0.0f, 0.1f, 0.2f, 0.0f);
    return true;
}
