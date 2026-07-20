#pragma once
#include <filesystem>
#include <map>
#include <string>
#include <vector>

#include "Mesh.h"
#include <assimp/Importer.hpp>
#include <assimp/scene.h>

#include "ShaderProgram.h"
#include "Texture2d.h"

struct BoundingBox
{
    Vector3f min, max;
};

class Model
{
public:
    Model(const std::string& path, bool flipUVs) { LoadModel(path, flipUVs); }
    Model(std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::vector<std::shared_ptr<Texture2d>> textures);
    void Draw(ShaderProgram& shaderProgram) const;
    const std::vector<Mesh>& GetMeshes() const;
    Vector3f GetLargestDiagonal() const;
    BoundingBox GetBoundingBox() const;
    void AddTexture(const std::string& path, const std::string& typeName);

private:
    void LoadModel(const std::string& path, bool flipUVs);
    void ProcessNode(aiNode* node, const aiScene* scene);
    Mesh ProcessMesh(aiMesh* mesh, const aiScene* scene);
    std::vector<std::shared_ptr<Texture2d>> LoadMaterialTextures(const aiScene* scene, aiMesh* mesh, aiTextureType type, const std::string& typeName);
    const std::filesystem::path FindCorrectPath(std::filesystem::path fileName) const;

private:
    std::vector<Mesh> m_Meshes;
    std::string m_Directory;
    std::map<std::string, std::shared_ptr<Texture2d>> m_LoadedTextures;

};
