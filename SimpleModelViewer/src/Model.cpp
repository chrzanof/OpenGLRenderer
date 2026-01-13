#include "Model.h"

#include <iostream>

#include "ShaderProgram.h"
#include "Texture2d.h"

void Model::Draw(ShaderProgram& shaderProgram, Texture2d& texture) const
{
	for (unsigned int i = 0; i < meshes.size(); i++)
	{
        meshes[i].Draw(shaderProgram, texture);
	}
}

const std::vector<Mesh>& Model::GetMeshes() const
{
    return meshes;
}

void Model::loadModel(const std::string& path)
{
    Assimp::Importer import;
    const aiScene * scene = import.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs);

    if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
    {
        std::cout << "ERROR::ASSIMP::" << import.GetErrorString() << std::endl;
        return;
    }
    directory = path.substr(0, path.find_last_of('/'));

    processNode(scene->mRootNode, scene);
}

void Model::processNode(aiNode* node, const aiScene* scene)
{
    for (unsigned int i = 0; i < node->mNumMeshes; i++)
    {
        aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
        meshes.push_back(processMesh(mesh, scene));
    }
    
    for (unsigned int i = 0; i < node->mNumChildren; i++)
    {
        processNode(node->mChildren[i], scene);
    }
}

Mesh Model::processMesh(aiMesh* mesh, const aiScene* scene)
{
    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;

    for (unsigned int i = 0; i < mesh->mNumVertices; i++)
    {
        Vector2f textCoords{ 0.0f, 0.0f };
        if (mesh->mTextureCoords[0])
        {
           
        	textCoords.x = mesh->mTextureCoords[0][i].x;
            textCoords.y = mesh->mTextureCoords[0][i].y;
            
        }
        Vertex vertex {
            Vector3f{mesh->mVertices[i].x, mesh->mVertices[i].y, mesh->mVertices[i].z},
            Vector3f{mesh->mNormals[i].x, mesh->mNormals[i].y, mesh->mNormals[i].z},
            textCoords
        };
    	vertices.push_back(vertex);
    }
    
    for (unsigned int i = 0; i < mesh->mNumFaces; i++)
    {
        aiFace face = mesh->mFaces[i];
        for (unsigned int j = 0; j < face.mNumIndices; j++)
            indices.push_back(face.mIndices[j]);
    }

    return Mesh{ vertices, indices };
}
