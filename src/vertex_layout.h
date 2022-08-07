#ifndef __VERTEX_LAYOUT_H__
#define __VERTEX_LAYOUT_H__

#include "common.h"

class VertexLayout {
public:
    static std::unique_ptr<VertexLayout> Create();
    ~VertexLayout();

    uint32_t Get() const { return vertexArrayObject; }
    void Bind() const;
    void SetAttrib(
        uint32_t attribIndex, int count,
        uint32_t type, bool normalized,
        size_t stride, uint64_t offset) const;
    void DisableAttrib(int attribIndex) const;

private:
    VertexLayout() {}
    void Init();
    uint32_t vertexArrayObject { 0 };
};

#endif
