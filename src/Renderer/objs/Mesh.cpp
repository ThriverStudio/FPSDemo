#include "Mesh.h"

#include <glad/glad.h>

#include <cmath>

void Mesh::Init(std::vector<Vertex>& vertices, std::vector<uint32_t>& indices, std::string textureIdx, std::unordered_map<std::string, Texture>& textureList, glm::mat4&& transform)
{
    m_VertCount = indices.size();
    m_TextureIdx = textureIdx;
    m_Transform = transform;
    m_TextureList = &textureList;
    
    {
        glGenVertexArrays(1, &m_Vao);
        glGenBuffers(1, &m_Vbo);
        glGenBuffers(1, &m_Ebo);

        glBindVertexArray(m_Vao);

        glBindBuffer(GL_ARRAY_BUFFER, m_Vbo);
        glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * vertices.size(), vertices.data(), GL_STATIC_DRAW);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_Ebo);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint32_t) * indices.size(), indices.data(), GL_STATIC_DRAW);

        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, pos));

        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, texCoord));

        glEnableVertexAttribArray(2);
        glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal));

        glBindVertexArray(0);
    }

    GenerateLightMap(vertices, indices);
}

void Mesh::Destroy()
{
    glDeleteVertexArrays(1, &m_Vao);
    glDeleteBuffers(1, &m_Vbo);
    glDeleteBuffers(1, &m_Ebo);
}

void Mesh::Render()
{
    Texture* texture = &(*m_TextureList)[m_TextureIdx];
    if (texture)
    {
        texture->Active(1);
        texture->Bind();
        m_LightMap.Active(2);
        m_LightMap.Bind();
    }

    glBindVertexArray(m_Vao);
    glDrawElements(GL_TRIANGLES, m_VertCount, GL_UNSIGNED_INT, 0);

    glBindTexture(GL_TEXTURE_2D, 0);
}

void Mesh::GenerateLightMap(std::vector<Vertex>& vertices, std::vector<uint32_t>& indices)
{
    Texture* diffuseMap = &(*m_TextureList)[m_TextureIdx];

    uint32_t width  = diffuseMap->GetWidth();
    uint32_t height = diffuseMap->GetHeight();

    uint32_t* pixels = new uint32_t[width * height];
    uint8_t* diffusePixels = new uint8_t[width * height * 4];
    uint32_t* diffuse32 = reinterpret_cast<uint32_t*>(diffusePixels);
    diffuseMap->Bind();
    glGetTexImage(GL_TEXTURE_2D, 0, GL_RGBA, GL_UNSIGNED_BYTE, diffusePixels);
    diffuseMap->Unbind();

    glm::vec3 lightDir = glm::normalize(glm::vec3(-1.0f, -1.0f, -1.0f));

    auto Edge = [](const glm::vec2& a, const glm::vec2& b, const glm::vec2& c)
    {
        return (c.x - a.x) * (b.y - a.y) - (c.y - a.y) * (b.x - a.x);
    };

    auto Uint32ToVec3 = [](uint32_t color)
    {
        return glm::vec3(color & 0xff, (color >> 8) & 0xff, (color >> 16) & 0xff) / 255.0f;
    };

    for (size_t i = 0; i < indices.size(); i += 3)
    {
        Vertex& v0 = vertices[indices[i]];
        Vertex& v1 = vertices[indices[i + 1]];
        Vertex& v2 = vertices[indices[i + 2]];

        glm::vec2 uv0 = v0.texCoord * glm::vec2(width, height);
        glm::vec2 uv1 = v1.texCoord * glm::vec2(width, height);
        glm::vec2 uv2 = v2.texCoord * glm::vec2(width, height);

        int minX = (int)std::floor(std::min(std::min(uv0.x, uv1.x), uv2.x));
        int maxX = (int)std::ceil (std::max(std::max(uv0.x, uv1.x), uv2.x));

        int minY = (int)std::floor(std::min(std::min(uv0.y, uv1.y), uv2.y));
        int maxY = (int)std::ceil (std::max(std::max(uv0.y, uv1.y), uv2.y));

        minX = std::max(minX, 0);
        minY = std::max(minY, 0);
        maxX = std::min(maxX, (int)width - 1);
        maxY = std::min(maxY, (int)height - 1);

        float area = Edge(uv0, uv1, uv2);
        if (area == 0.0f) 
            continue;

        for (int y = minY; y <= maxY; y++)
        {
            for (int x = minX; x <= maxX; x++)
            {
                glm::vec2 p(x + 0.5f, y + 0.5f);

                float w0 = Edge(uv1, uv2, p) / area;
                float w1 = Edge(uv2, uv0, p) / area;
                float w2 = Edge(uv0, uv1, p) / area;

                if (w0 >= 0 && w1 >= 0 && w2 >= 0)
                {
                    glm::vec3 normal = w0 * v0.normal + w1 * v1.normal + w2 * v2.normal;

                    normal = glm::normalize(normal);

                    float diffuse = std::max(glm::dot(normal, -lightDir), 0.0f);
                    glm::vec3 color = Uint32ToVec3(diffuse32[y * width + x]);
                    color = color * diffuse;

                    uint8_t r = (uint8_t)(color.r * 255.0f);
                    uint8_t g = (uint8_t)(color.g * 255.0f);
                    uint8_t b = (uint8_t)(color.b * 255.0f);

                    pixels[y * width + x] = (255 << 24) | (b << 16) | (g << 8) | r;
                }
            }
        }
    }

    m_LightMap.Init(width, height, pixels);

    delete[] diffusePixels;
    delete[] pixels;
}