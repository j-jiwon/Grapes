#include "program.h"

std::unique_ptr<Program> Program::Create(const std::vector<std::shared_ptr<Shader>>& shaders){
    auto program = std::unique_ptr<Program>(new Program());
    if(!program->Link(shaders))
        return nullptr;
    return std::move(program);
}

Program::~Program() {
    if (programId)
        glDeleteProgram(programId);
}

bool Program::Link(const std::vector<std::shared_ptr<Shader>>& shaders) {
    programId = glCreateProgram();
    for (auto& shader: shaders)
        glAttachShader(programId, shader->Get());
    glLinkProgram(programId);

    int success = 0;
    glGetProgramiv(programId, GL_LINK_STATUS, &success);
    if (!success) {
        char infoLog[1024];
        glGetProgramInfoLog(programId, 1024, nullptr, infoLog);
        SPDLOG_ERROR("failed to link program: {}", infoLog);
        return false;
    }
    return true;
}
