#ifndef __CONTEXT_H__
#define __CONTEXT_H__

#include "common.h"
#include "shader.h"
#include "program.h"

class Context {
public:
    static std::unique_ptr<Context> Create();
    void Render();    
private:
    Context() {}
    bool Init();
    std::unique_ptr<Program> program;
};

#endif
