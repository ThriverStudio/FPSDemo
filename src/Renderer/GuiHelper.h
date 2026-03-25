#pragma once

#include "Window/Window.h"

#include <memory>

class GuiHelper
{
public:
    static void Init(std::shared_ptr<Window> window, bool dockspace = true);
    static void StartFrame();
    static void EndFrame();
    static void Update(std::shared_ptr<Window> window);
    static void Shutdown();
};