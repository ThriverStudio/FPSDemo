#pragma once

#include "objs/Camera.h"
#include "objs/Model.h"
#include "objs/Shader.h"
#include "objs/Framebuffer.h"
#include "objs/Skybox.h"

#include "Window/Window.h"

#include <memory>

class Renderer
{
public:
    Renderer(std::shared_ptr<Window> window);
    ~Renderer();

    void Render();
    void Update();

private:
    std::shared_ptr<Window> m_Window;
    float m_Delta, m_LastTime;
    Camera m_Camera;
    Shader m_SceneShader, m_FinalPassShader;
    Model m_Model;

    Framebuffer m_Fb;
    uint32_t m_QuadVao, m_QuadVbo;
};
