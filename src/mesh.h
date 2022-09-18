#ifndef __MESH_H__
#define __MESH_H__

#include "common.h"
#include "buffer.h"
#include "vertex_layout.h"
#include "texture.h"
#include "program.h"


struct Vertex {
    glm::vec3 position;
    glm::vec3 normal;
    glm::vec2 texCoord;
};

class Material {
public:
    static std::unique_ptr<Material> Create() {
        return std::unique_ptr<Material>(new Material());
    }
    std::shared_ptr<Texture> diffuse;
    std::shared_ptr<Texture> specular;
    float shininess { 32.0f };
    void SetToProgram(const Program* program) const;

private:
    Material() {}
};

class Mesh {
public:
    static std::unique_ptr<Mesh> Create(
        const std::vector<Vertex>& vertices,
        const std::vector<uint32_t>& indices,
        uint32_t primitiveType);
    static std::unique_ptr<Mesh> CreateBox();

    const VertexLayout* GetVertexLayout() const {
        return vertexLayout.get();
    }
    std::shared_ptr<Buffer> GetVertexBuffer() const { return vertexBuffer; }
    std::shared_ptr<Buffer> GetIndexBuffer() const { return indexBuffer; }

    void SetMaterial(std::shared_ptr<Material> _material) { material = _material; }
    std::shared_ptr<Material> GetMaterial() const { return material; }

    void Draw(const Program* program) const;

private:
    Mesh() {}
    void Init(
        const std::vector<Vertex>& vertices,
        const std::vector<uint32_t>& indices,
        uint32_t _primitiveType);

    uint32_t primitiveType { GL_TRIANGLES };
    std::unique_ptr<VertexLayout> vertexLayout;
    std::shared_ptr<Buffer> vertexBuffer;
    std::shared_ptr<Buffer> indexBuffer;
    std::shared_ptr<Material> material;
};

#endif // __MESH_H__
