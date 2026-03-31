#include "Renderer.h"

#include "GuiHelper.h"

#include <imgui/imgui.h>

#include <stb/stb_image.h>
#include <iostream>

Renderer::Renderer(std::shared_ptr<Window> window)
{
    m_Window = window;

    m_Model.Init("assets/meshes/experiment/experiment.gltf");

    m_Shader.Init("assets/shaders/default.glsl");

    m_Skybox.Init("assets/skybox/1.hdr");

    GuiHelper::Init(m_Window, false);
}

Renderer::~Renderer()
{
    GuiHelper::Shutdown();

    m_Skybox.Destroy();
    m_Model.Destroy();
    m_Shader.Destroy();
}

void Renderer::Render()
{
    // Main Rendering
    glEnable(GL_CULL_FACE);
    glEnable(GL_DEPTH_TEST);
    glFrontFace(GL_CW);
    glCullFace(GL_BACK);
    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    m_Shader.Bind();
    m_Shader.PutMat4("proj", m_Camera.GetProjMat());
    m_Shader.PutMat4("view", m_Camera.GetViewMat());
    m_Shader.PutTex("lightMap", 0);

    m_Model.Render("model", m_Shader);

    m_Skybox.Render(m_Camera);
 
    // ImGui rendering
    GuiHelper::StartFrame();

    ImGui::Begin("Stats");

    ImGui::Text("Delta Time :- %0.2fms", m_Delta * 1000);
    ImGui::Text("%.0f FPS", 1/m_Delta);

    ImGui::End();

    GuiHelper::EndFrame();
    GuiHelper::Update(m_Window);
}

void Renderer::Update()
{
    float crntTime = (float)glfwGetTime();
    m_Delta = crntTime - m_LastTime;
    m_LastTime = crntTime;

    glViewport(0, 0, m_Window->GetWindowInfo().width, m_Window->GetWindowInfo().height);

    if(m_Window->GetWindowInfo().width > 0 && m_Window->GetWindowInfo().height > 0)
        m_Camera.Update(*m_Window, (float)m_Window->GetWindowInfo().width/(float)m_Window->GetWindowInfo().height, m_Delta);
}
