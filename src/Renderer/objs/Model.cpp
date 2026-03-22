#include "Model.h"

#include "Utils.h"

#include <stb/stb_image.h>

void Model::Init(std::string path)
{
    Assimp::Importer importer;
    const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_OptimizeMeshes | aiProcess_JoinIdenticalVertices | 
                                                     aiProcess_GenNormals | aiProcess_SplitLargeMeshes);

    if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
    {
        FATAL("Failed to load model. Reason by assimp :- " + std::string(importer.GetErrorString()));
    }

    m_Dir = path.substr(0, path.find_last_of('/'));
    ProcessNode(scene->mRootNode, scene);
}

void Model::Destroy()
{
    for (auto& mesh : m_Meshes)
        mesh.Destroy();

    for (auto& tex : m_Textures)
        tex.Destroy();
}

void Model::Render()
{
    for (auto& mesh : m_Meshes)
        mesh.Render();
}

void Model::ProcessNode(aiNode* node, const aiScene* scene)
{
    for (uint32_t i = 0; i < node->mNumMeshes; i++)
    {
        aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
        m_Meshes.push_back(ProcessMesh(mesh, scene));
    }

    for (uint32_t i = 0; i < node->mNumChildren; i++)
    {
        ProcessNode(node->mChildren[i], scene);
    }
}

Mesh Model::ProcessMesh(aiMesh* mesh, const aiScene* scene)
{
    std::vector<Vertex> vertices;
    std::vector<uint32_t> indices;
    Texture* diffuseTexture = nullptr;

    for (uint32_t i = 0; i < mesh->mNumVertices; i++)
    {
        Vertex vertex{};
        vertex.pos = glm::vec3(mesh->mVertices[i].x, mesh->mVertices[i].y, mesh->mVertices[i].z);
        vertex.texCoord = mesh->mTextureCoords[0] ? 
                          glm::vec2(mesh->mTextureCoords[0][i].x, mesh->mTextureCoords[0][i].y) : 
                          glm::vec2(0.0f, 0.0f);
        if (mesh->HasNormals())
        {
            vertex.normal = glm::vec3(mesh->mNormals[i].x, mesh->mNormals[i].y, mesh->mNormals[i].z);
        }
        else
        {
            vertex.normal = glm::vec3(0.0f);
        }

        vertices.push_back(vertex);
    }

    for (uint32_t i = 0; i < mesh->mNumFaces; i++)
    {
        aiFace face = mesh->mFaces[i];
        for (uint32_t j = 0; j < face.mNumIndices; j++)
        {
            indices.push_back(face.mIndices[j]);
        }
    }

    if (mesh->mMaterialIndex >= 0)
    {
        aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
        std::vector<Texture> textures = LoadMaterialTextures(material, aiTextureType_DIFFUSE); // Only diffuse textures for now
        
        if (!textures.empty())
        {
            m_Textures.push_back(textures[0]);
            diffuseTexture = &m_Textures.back();
        }
    }

    Mesh newMesh;
    newMesh.Init(vertices, indices, diffuseTexture);
    return newMesh;
}

std::vector<Texture> Model::LoadMaterialTextures(aiMaterial* mat, aiTextureType type)
{
    std::vector<Texture> textures;

    for (uint32_t i = 0; i < mat->GetTextureCount(type); i++)
    {
        aiString str;
        mat->GetTexture(type, i, &str);

        std::string path = m_Dir + std::string("/") + std::string(str.C_Str());

        // stbi_set_flip_vertically_on_load(true);
        int width, height, channels;
        auto* data = stbi_load(path.c_str(), &width, &height, &channels, 4);
        if(!data)
        {
            FATAL("Failed to load model texture. Reason by stb image :- " + std::string(stbi_failure_reason()))
        }

        Texture texture;
        texture.Init(width, height, data);
        textures.push_back(texture);
    }

    return textures;
}