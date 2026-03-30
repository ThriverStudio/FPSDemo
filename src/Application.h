#pragma once

#include "Window/Window.h"
#include "Renderer/Renderer.h"
#include <slog/slog.h>

#include <memory>

class Application
{
public:
    Application();
    ~Application();
    
    void Run();

private:
    void Render();
    void Update();

private:
    std::shared_ptr<Window> m_Window;
    std::shared_ptr<Renderer> m_Renderer;
};