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

void Program::Use() const {
    glUseProgram(programId);
}

void Program::SetUniform(const std::string& name, int value) const {
  auto loc = glGetUniformLocation(programId, name.c_str());
  glUniform1i(loc, value);
}

void Program::SetUniform(const std::string& name, float value) const {
    auto loc = glGetUniformLocation(programId, name.c_str());
    glUniform1f(loc, value);
}

void Program::SetUniform(const std::string& name, const glm::vec3& value) const {
    auto loc = glGetUniformLocation(programId, name.c_str());
    glUniform3fv(loc, 1, glm::value_ptr(value));
}

void Program::SetUniform(const std::string& name,
  const glm::mat4& value) const {
  auto loc = glGetUniformLocation(programId, name.c_str());
  glUniformMatrix4fv(loc, 1, GL_FALSE, glm::value_ptr(value));
}
