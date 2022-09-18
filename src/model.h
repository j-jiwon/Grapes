#ifndef __MODEL_H__
#define __MODEL_H__

#include "common.h"
#include "mesh.h"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

class Model {
public:
    static std::unique_ptr<Model> Load(const std::string& filename);

    int GetMeshCount() const { return (int)meshes.size(); }
    std::shared_ptr<Mesh> GetMesh(int index) const { return meshes[index]; }
    void Draw(const Program* program) const;

private:
    Model() {}
    bool LoadByAssimp(const std::string& filename);
    void ProcessMesh(aiMesh* mesh, const aiScene* scene);
    void ProcessNode(aiNode* node, const aiScene* scene);
        
    std::vector<std::shared_ptr<Mesh>> meshes;
    std::vector<std::shared_ptr<Material>> materials;
};

#endif // __MODEL_H__
