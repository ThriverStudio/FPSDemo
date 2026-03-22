#include "Renderer.h"

#include "GuiHelper.h"

#include <imgui/imgui.h>

#include <stb/stb_image.h>
#include <iostream>

Renderer::Renderer(std::shared_ptr<Window> window)
{
    m_Window = window;

    m_Model.Init("assets/meshes/coffee_table/coffee_table.gltf");

    m_Shader.Init("assets/shaders/default.glsl");

    m_Fb.Init(m_Window->GetWindowInfo().width, m_Window->GetWindowInfo().height);

    m_Skybox.Init("assets/skybox/1.hdr");

    GuiHelper::Init(*m_Window);
}

Renderer::~Renderer()
{
    GuiHelper::Shutdown();

    m_Skybox.Destroy();
    m_Model.Destroy();
    m_Shader.Destroy();
    m_Fb.Destroy();
}

void Renderer::Render()
{
    GuiHelper::StartFrame();

    ImGui::Begin("Scene");

    m_Fb.Resize(ImGui::GetContentRegionAvail().x, ImGui::GetContentRegionAvail().y);
    m_Fb.Bind();
    glViewport(0, 0, ImGui::GetContentRegionAvail().x, ImGui::GetContentRegionAvail().y);
    
    // Main Rendering
    {
        glEnable(GL_CULL_FACE);
        glEnable(GL_DEPTH_TEST);
        glFrontFace(GL_CW);
        glCullFace(GL_BACK);
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        m_Shader.Bind();
        m_Shader.PutMat4("proj", m_Camera.GetProjMat());
        m_Shader.PutMat4("view", m_Camera.GetViewMat());
        m_Shader.PutMat4("model", glm::mat4(1.0f));
        m_Shader.PutTex("tex", 0);

        m_Model.Render();

        m_Skybox.Render(m_Camera);
    }
    
    m_Fb.Unbind();
    
    ImGui::Image(reinterpret_cast<ImTextureID>(static_cast<uintptr_t>(m_Fb.GetColorAttachment().GetHandle())), 
                        ImVec2(m_Fb.GetColorAttachment().GetWidth(), m_Fb.GetColorAttachment().GetHeight()), 
                        ImVec2(0, 1), ImVec2(1, 0));

    ImGui::End();

    ImGui::Begin("Stats");

    ImGui::Text("Delta Time :- %0.2fms", m_Delta * 1000);
    ImGui::Text("%.0f FPS", 1/m_Delta);

    ImGui::End();

    GuiHelper::EndFrame();

    glViewport(0, 0, m_Window->GetWindowInfo().width, m_Window->GetWindowInfo().height);
    GuiHelper::Update(*m_Window);
}

void Renderer::Update()
{
    float crntTime = (float)glfwGetTime();
    m_Delta = crntTime - m_LastTime;
    m_LastTime = crntTime;

    if(m_Window->GetWindowInfo().width > 0 && m_Window->GetWindowInfo().height > 0)
        m_Camera.Update(*m_Window, (float)m_Window->GetWindowInfo().width/(float)m_Window->GetWindowInfo().height, m_Delta);
}
