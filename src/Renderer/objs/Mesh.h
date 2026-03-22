#pragma once

#include <glm/glm.hpp>
#include <vector>
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
    void Init(std::vector<Vertex>& vertices, std::vector<uint32_t>& indices, Texture* texture);
    void Destroy();

    void Render();

private:
    uint32_t m_VertCount;
    uint32_t m_Vao, m_Vbo, m_Ebo;
    Texture* m_Texture;
};