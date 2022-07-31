#include "context.h"

std::unique_ptr<Context> Context::Create() {
    auto context = std::unique_ptr<Context>(new Context());
    if (!context->Init())
        return nullptr;
    return std::move(context);
}

void Context::Render() {
    glClear(GL_COLOR_BUFFER_BIT);

    glUseProgram(program->Get());
    glDrawArrays(GL_POINTS, 0, 1);
}

bool Context::Init() {
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

    glClearColor(1.0f, 1.0f, 0.0f, 0.0f);

    uint32_t vao = 0;
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    return true;
}
