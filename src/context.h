#ifndef __CONTEXT_H__
#define __CONTEXT_H__

#include "common.h"
#include "shader.h"
#include "program.h"
#include "buffer.h"
#include "vertex_layout.h"

class Context {
public:
    static std::unique_ptr<Context> Create();
    void Render();    
private:
    Context() {}
    bool Init();
    std::unique_ptr<Program> program;

    std::unique_ptr<VertexLayout> vertexLayout;
    std::unique_ptr<Buffer> vertexBuffer;
    std::unique_ptr<Buffer> indexBuffer;
};

#endif
