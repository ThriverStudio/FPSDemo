#pragma once

#include <glm/glm.hpp>
#include <map>
#include <string>
#include <cstdint>

#include "Texture.h"

struct Vertex
{
    glm::vec3 pos;
    glm::vec2 texCoord;
    glm::vec3 normal;
};

class Mesh
{
public:
    void Init(std::vector<Vertex>& vertices, std::vector<uint32_t>& indices, std::string textureIdx, std::unordered_map<std::string, Texture>& textureList,glm::mat4&& transform);
    void Destroy();

    void Render();
    inline glm::mat4& GetTransform() { return m_Transform; }

private:
    void GenerateLightMap(std::vector<Vertex>& vertices, std::vector<uint32_t>& indices);

private:
    uint32_t m_VertCount;
    uint32_t m_Vao, m_Vbo, m_Ebo;
    std::string m_TextureIdx;
    glm::mat4 m_Transform = glm::mat4(1.0f);
    
    Texture m_LightMap;
    std::unordered_map<std::string, Texture>* m_TextureList = nullptr;
};