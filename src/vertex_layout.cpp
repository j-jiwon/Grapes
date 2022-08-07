#include "vertex_layout.h"

std::unique_ptr<VertexLayout> VertexLayout::Create() {
    auto _vertexLayout = std::unique_ptr<VertexLayout>(new VertexLayout());
    _vertexLayout->Init();
    return std::move(_vertexLayout);
}

VertexLayout::~VertexLayout() {
    if (vertexArrayObject) {
        glDeleteVertexArrays(1, &vertexArrayObject);
    }
}

void VertexLayout::Bind() const {
    glBindVertexArray(vertexArrayObject);
}

void VertexLayout::SetAttrib(
    uint32_t attribIndex, int count,
    uint32_t type, bool normalized,
    size_t stride, uint64_t offset) const {
    glEnableVertexAttribArray(attribIndex);
    glVertexAttribPointer(attribIndex, count,
        type, normalized, stride, (const void*)offset);
}

void VertexLayout::Init() {
    glGenVertexArrays(1, &vertexArrayObject);
    Bind();
}
