#pragma once

#include <string>
#include <cstdint>
#include <map>

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

class Shader
{
public:
    void Init(std::string path);
    void Destroy();

    void Bind();
    void Unbind();

    void PutTex(const std::string& name, int texIndex);
    void PutVec2(const std::string& name, glm::vec2& data);
    void PutVec3(const std::string& name, glm::vec3& data);
    void PutVec4(const std::string& name, glm::vec4& data);
    void PutIVec2(const std::string& name, glm::ivec2& data);
    void PutIVec3(const std::string& name, glm::ivec3& data);
    void PutIVec4(const std::string& name, glm::ivec4& data);
    void PutDVec2(const std::string& name, glm::dvec2& data);
    void PutDVec3(const std::string& name, glm::dvec3& data);
    void PutDVec4(const std::string& name, glm::dvec4& data);
    void PutMat3(const std::string& name, glm::mat3& data);
    void PutMat4(const std::string& name, glm::mat4& data);
    void PutInt(const std::string& name, int data);
    void PutFloat(const std::string& name, float data);
private:
    void ReadFile(std::string path);
    uint32_t GetLocation(const std::string& name);

private:
    std::string vSrc, fSrc;
    uint32_t m_vID, m_fID, m_pID;
    std::unordered_map<std::string, uint32_t> m_Map;
};
