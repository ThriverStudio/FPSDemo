#include "Camera.h"

#include "Window/Input.h"

void Camera::Update(Window& m_Window, float aspectRatio, float deltaTime, float fov)
{
    if(Input::IsKeyPressed(m_Window, GLFW_KEY_W))
        m_CamPos += m_CamFront * deltaTime * 2.0f;
    if(Input::IsKeyPressed(m_Window, GLFW_KEY_S))
        m_CamPos -= m_CamFront * deltaTime * 2.0f;
    if(Input::IsKeyPressed(m_Window, GLFW_KEY_A))
        m_CamPos += m_CamRight * deltaTime * 2.0f;
    if(Input::IsKeyPressed(m_Window, GLFW_KEY_D))
        m_CamPos -= m_CamRight * deltaTime * 2.0f;
    if(Input::IsKeyPressed(m_Window, GLFW_KEY_SPACE))
        m_CamPos += GetWorldUp() * deltaTime * 2.0f;
    if(Input::IsKeyPressed(m_Window, GLFW_KEY_LEFT_SHIFT))
        m_CamPos -= GetWorldUp() * deltaTime * 2.0f;

    if(Input::IsMBPressed(m_Window, GLFW_MOUSE_BUTTON_RIGHT))
    {
        double mouseX, mouseY;
        glfwGetCursorPos(m_Window.GetHandle(), &mouseX, &mouseY);

        if (firstMouse) {
            glfwSetInputMode(m_Window.GetHandle(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);
            lastMouseX = mouseX;
            lastMouseY = mouseY;
            firstMouse = false;
        }

        float offsetX = (mouseX - lastMouseX) * m_Sensitivity;
        float offsetY = (lastMouseY - mouseY) * m_Sensitivity;

        lastMouseX = mouseX;
        lastMouseY = mouseY;

        m_Yaw += offsetX;
        m_Pitch += offsetY;

        if (m_Pitch > 89.0f) m_Pitch = 89.0f;
        if (m_Pitch < -89.0f) m_Pitch = -89.0f;

        glm::vec3 front;
        front.x = glm::cos(glm::radians(m_Yaw)) * glm::cos(glm::radians(m_Pitch));
        front.y = glm::sin(glm::radians(m_Pitch));
        front.z = glm::sin(glm::radians(m_Yaw)) * glm::cos(glm::radians(m_Pitch));
        m_CamFront = glm::normalize(front);
    }
    else if(Input::IsMBReleased(m_Window, GLFW_MOUSE_BUTTON_RIGHT))
    {
        Input::ShowMouse(m_Window);
        firstMouse = true;
    }

    m_CamRight = glm::normalize(glm::cross(GetWorldUp(), m_CamFront));
    m_CamUp = glm::normalize(glm::cross(m_CamFront, m_CamRight));

    m_Proj = glm::perspective(glm::radians(fov), aspectRatio, 0.01f, 1000.0f);
    m_View = glm::lookAt(m_CamPos, m_CamPos + m_CamFront, GetWorldUp());
}