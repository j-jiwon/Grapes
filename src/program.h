#ifndef __PROGRAM_H__
#define __PROGRAM_H__

#include "common.h"
#include "shader.h"

class Program {
public:
    static std::unique_ptr<Program> Create(const std::vector<std::shared_ptr<Shader>>& shaders);
    static std::unique_ptr<Program> Create(const std::string& vertexShaderFilename, 
                                           const std::string& fragmentShaderFilename);

    ~Program();
    uint32_t Get() const { return programId; }
    void Use() const;
    void SetUniform(const std::string& name, int value) const;
    void SetUniform(const std::string& name, float value) const;
    void SetUniform(const std::string& name, const glm::vec3& value) const;
    void SetUniform(const std::string& name, const glm::vec4& value) const;
    void SetUniform(const std::string& name, const glm::mat4& value) const;

private:
    Program() {}
    bool Link(const std::vector<std::shared_ptr<Shader>>& shaders);
    uint32_t programId { 0 };
};

#endif 
