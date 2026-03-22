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
    void Init(std::vector<Vertex>& vertices, std::vector<uint32_t>& indices, Texture* texture, glm::mat4&& transform);
    void Destroy();

    void Render();
    inline const glm::mat4& GetTransform() { return m_Transform; }

private:
    uint32_t m_VertCount;
    uint32_t m_Vao, m_Vbo, m_Ebo;
    Texture* m_Texture;
    glm::mat4 m_Transform = glm::mat4(1.0f);
};