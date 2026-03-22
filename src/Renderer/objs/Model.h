#pragma once

#include "Mesh.h"
#include "Texture.h"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include <string>
#include <vector>
#include <iostream>

class Model
{
public:
    void Init(std::string path);
    void Destroy();

    void Render();

private:
    std::string m_Dir;
    std::vector<Mesh> m_Meshes;
    std::vector<Texture> m_Textures;

    void ProcessNode(aiNode* node, const aiScene* scene);
    Mesh ProcessMesh(aiMesh* mesh, const aiScene* scene);
    std::vector<Texture> LoadMaterialTextures(aiMaterial* mat, aiTextureType type);
};