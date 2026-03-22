#pragma once

#include "Window/Window.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

class Camera
{
public:
    void Update(Window& window, float aspectRatio, float deltaTime, float fov = 90.0f);

    inline const glm::mat4& GetViewMat() { return m_View; }
    inline const glm::mat4& GetProjMat() { return m_Proj; }
    inline const glm::vec3& GetPos() { return m_CamPos; }
    inline const glm::vec3& GetFront() { return m_CamFront; }
    inline const glm::vec3& GetRight() { return m_CamRight; }
    inline const glm::vec3& GetUp() { return m_CamUp; }
    inline glm::vec3 GetWorldUp() { return glm::vec3(0.0f, 1.0f, 0.0f); }
private:
    glm::vec3 m_CamPos = glm::vec3(0, 0, 2);
    glm::vec3 m_CamFront = glm::vec3(0.0f, 0.0f, -1.0f);
    glm::vec3 m_CamRight = glm::vec3(0.0f);
    glm::vec3 m_CamUp = glm::vec3(0.0f);
    glm::mat4 m_Proj = glm::mat4(1.0f);
    glm::mat4 m_View = glm::mat4(1.0f);
    float m_Yaw = -90.0f; 
    float m_Pitch = 0.0f; 
    float m_Sensitivity = 0.1f; 
    double lastMouseX = 400, lastMouseY = 300;
    bool firstMouse = true;
};