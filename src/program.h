#ifndef __PROGRAM_H__
#define __PROGRAM_H__

#include "common.h"
#include "shader.h"

class Program {
public:
    static std::unique_ptr<Program> Create(const std::vector<std::shared_ptr<Shader>>& shaders);
    ~Program();
    uint32_t Get() const { return programId; }
private:
    Program() {}
    bool Link(const std::vector<std::shared_ptr<Shader>>& shaders);
    uint32_t programId { 0 };
};

#endif 
