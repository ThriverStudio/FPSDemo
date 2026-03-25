#pragma once

#include "Mesh.h"
#include "Texture.h"
#include "Shader.h"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include <string>
#include <vector>
#include <map>
#include <iostream>

class Model
{
public:
    void Init(std::string path);
    void Destroy();

    void Render(std::string attributeName, Shader& shader);

private:
    std::string m_Dir;
    std::vector<Mesh> m_Meshes;
    std::unordered_map<std::string, Texture> m_Textures;

    void ProcessNode(aiNode* node, const aiScene* scene, glm::mat4 transform = glm::mat4(1.0f));
    Mesh ProcessMesh(aiMesh* mesh, const aiScene* scene, glm::mat4 transform);
    std::vector<std::string> GetMaterialTextures(aiMaterial* mat, aiTextureType type);
};