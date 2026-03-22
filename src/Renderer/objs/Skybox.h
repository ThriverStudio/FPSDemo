#pragma once

#include <string>

#include "Shader.h"
#include "Camera.h"

class Skybox
{
public:
    void Init(const char* path);
    void Destroy();

    void Render(Camera& camera);

private:
    Shader m_Shader;
    uint32_t m_TexId;
    uint32_t m_Vao, m_Vbo;
};
