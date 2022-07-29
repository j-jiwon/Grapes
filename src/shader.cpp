#include "shader.h"

std::unique_ptr<Shader> Shader::CreateFromFile(const std::string& filename, GLenum shaderType){
    auto shader = std::unique_ptr<Shader>(new Shader());
    if (!shader->LoadFile(filename, shaderType))
        return nullptr;
    return std::move(shader);
}

Shader::~Shader(){
    if (shaderId)
        glDeleteShader(shaderId);
}

bool Shader::LoadFile(const std::string& filename, GLenum shaderType){
    auto result = LoadTextFile(filename);
    if (!result.has_value())
        return false;

    auto& code = result.value();
    const char* codePtr = code.c_str();
    int32_t codeLength = (int32_t)code.length();

    shaderId = glCreateShader(shaderType);
    glShaderSource(shaderId, 1, (const GLchar* const*)&codePtr, &codeLength);
    glCompileShader(shaderId);

    int success = 0;
    glGetShaderiv(shaderId, GL_COMPILE_STATUS, &success);
    if (!success) {
        char infoLog[1024];
        glGetShaderInfoLog(shaderId, 1024, nullptr, infoLog);
        SPDLOG_ERROR("failed to compile shader: \"{}\"", filename);
        SPDLOG_ERROR("reason: {}", infoLog);
        return false;
    }
    return true;
}
