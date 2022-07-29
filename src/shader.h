#ifndef __SHADER_H__
#define __SHADER_H__

#include "common.h"
class Shader {
public:
    static std::unique_ptr<Shader> CreateFromFile(const std::string& filename, GLenum shaderType);
    ~Shader();
    uint32_t Get() const { return shaderId; }

private:
    Shader() {};
    bool LoadFile(const std::string& filename, GLenum shaderType);
    uint32_t shaderId = { 0 };
};

#endif
