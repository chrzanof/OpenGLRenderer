#include "Model.h"

#include <filesystem>
#include <iostream>
#include <assimp/postprocess.h>
#include "ShaderProgram.h"
#include "Texture2d.h"

Model::Model(std::vector<Vertex> vertices, std::vector<unsigned int> indices,
    std::vector<std::shared_ptr<Texture2d>> textures): m_Directory("none")
{
    m_Meshes.emplace_back(std::move(vertices), std::move(indices), std::move(textures));
}

void Model::Draw(ShaderProgram& shaderProgram) const
{
	for (unsigned int i = 0; i < m_Meshes.size(); i++)
	{
        m_Meshes[i].Draw(shaderProgram);
	}
}

const std::vector<Mesh>& Model::GetMeshes() const
{
    return m_Meshes;
}

Vector3f Model::GetLargestDiagonal() const
{
    auto bBox = GetBoundingBox();
    return bBox.max - bBox.min;
}

BoundingBox Model::GetBoundingBox() const
{
    float minX{ 0 }, minY{ 0 }, minZ{ 0 },
        maxX{ 0 }, maxY{ 0 }, maxZ{ 0 };
    for (auto& mesh : m_Meshes)
    {
        for (auto& vertex : mesh.GetVerticesData())
        {
            minX = std::min(minX, vertex.position.x);
            minY = std::min(minY, vertex.position.y);
            minZ = std::min(minZ, vertex.position.z);

            maxX = std::max(maxX, vertex.position.x);
            maxY = std::max(maxY, vertex.position.y);
            maxZ = std::max(maxZ, vertex.position.z);
        }
    }
    return{ .min{minX, minY, minZ}, .max{maxX, maxY, maxZ} };
}

void Model::LoadModel(const std::string& path)
{
    Assimp::Importer import;
    const aiScene * scene = import.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs);

    if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
    {
        std::cout << "ERROR::ASSIMP::" << import.GetErrorString() << std::endl;
        return;
    }
    m_Directory = path.substr(0, path.find_last_of('/'));

    ProcessNode(scene->mRootNode, scene);
}

void Model::AddTexture(const std::string& path)
{
    std::shared_ptr<Texture2d> texture2d;
    if(auto search = m_LoadedTextures.find(path); search == m_LoadedTextures.end())
    {
        texture2d = std::make_shared<Texture2d>(path);
        m_LoadedTextures.insert(std::make_pair(path, texture2d));
    }
    else
    {
        texture2d = search->second;
    }
    for(int i = 0; i < m_Meshes.size(); i++)
    {
        m_Meshes[i].m_Textures.insert(m_Meshes[i].m_Textures.begin(), texture2d);
    }
}

void Model::ProcessNode(aiNode* node, const aiScene* scene)
{
    for (unsigned int i = 0; i < node->mNumMeshes; i++)
    {
        aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
        m_Meshes.push_back(ProcessMesh(mesh, scene));
    }
    
    for (unsigned int i = 0; i < node->mNumChildren; i++)
    {
        ProcessNode(node->mChildren[i], scene);
    }
}

Mesh Model::ProcessMesh(aiMesh* mesh, const aiScene* scene)
{
    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;
    std::vector<std::shared_ptr<Texture2d>> textures;

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

    if(mesh->mMaterialIndex >= 0)
    {
        
        textures = LoadMaterialTextures(scene, mesh,aiTextureType_DIFFUSE, "texture_diffuse");
    }

    return Mesh{ vertices, indices, textures };
}

std::vector<std::shared_ptr<Texture2d>> Model::LoadMaterialTextures(const aiScene* scene, aiMesh* mesh, aiTextureType type, std::string typeName)
{
    aiMaterial* mat = scene->mMaterials[mesh->mMaterialIndex];
    std::vector<std::shared_ptr<Texture2d>> textures;
    for (unsigned int i = 0; i < mat->GetTextureCount(type); i++)
    {
        aiString str;
        mat->GetTexture(type, i, &str);
        std::filesystem::path texturePath = str.C_Str();
        std::filesystem::path modelDir = m_Directory;
        modelDir = modelDir.parent_path();
        modelDir /= texturePath;

        // check if texture is already loaded
        auto search = m_LoadedTextures.find(modelDir.string());
        if(search != m_LoadedTextures.end())
        {
            textures.push_back(search->second);
            continue;
        }
        // check if texture is embedded (path: "*N")
        if (str.C_Str()[0] == '*')
        {
            int index = str.C_Str()[1] - '0';
            const aiTexture* texture = scene->mTextures[index];
            auto texture2d = std::make_shared<Texture2d>(texture);
            m_LoadedTextures.insert(std::make_pair(modelDir.string(), texture2d));
            textures.push_back(texture2d);
            continue;
        }
        // check if path to external texture is correct
        if (!std::filesystem::exists(modelDir))
        {
            std::filesystem::path fileName = texturePath.filename();
            modelDir = FindCorrectPath(fileName);
        }
        // if path was found then load texture
        if (modelDir != "")
        {
            auto texture2d = std::make_shared<Texture2d>(modelDir.string());
            m_LoadedTextures.insert(std::make_pair(modelDir.string(), texture2d));
            textures.push_back(texture2d);
        }

    }
    return textures;
}

const std::filesystem::path Model::FindCorrectPath(std::filesystem::path fileName) const
{
    std::filesystem::path modelBasePath = std::filesystem::path(m_Directory).parent_path();
    
    std::vector < std::string > folders = {
		"textures",
    	"Textures",
        "texture",
        "Texture",
        "maps",
        "Maps",
        "materials",
        "Materials",
		"images",
		"Images",
        "pbr",
		"PBR"
    };
    std::vector<std::filesystem::path> candidates;

    candidates.emplace_back(modelBasePath / fileName);
    candidates.emplace_back(modelBasePath.parent_path() / fileName);
    candidates.emplace_back(modelBasePath.parent_path().parent_path() / fileName);
    

    for (const auto & folder : folders)
    {
        candidates.emplace_back(modelBasePath / folder /fileName);
        candidates.emplace_back(modelBasePath.parent_path() / folder / fileName);
        candidates.emplace_back(modelBasePath.parent_path().parent_path() / folder / fileName);
    }

    for (const auto & candidate : candidates)
    {
        if (std::filesystem::exists(candidate))
            return candidate;
    }
 
    return "";
}
