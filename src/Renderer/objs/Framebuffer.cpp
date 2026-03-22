#include "Framebuffer.h"

#include <glad/glad.h>

#include "Utils.h"

void Framebuffer::Init(int32_t width, int32_t height)
{
    glGenFramebuffers(1, &m_Handle);
    glBindFramebuffer(GL_FRAMEBUFFER, m_Handle);

    m_ColAttach.Init(width, height);
    m_DepthAttach.Init(width, height, nullptr, true);

    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_ColAttach.GetHandle(), 0);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_TEXTURE_2D, m_DepthAttach.GetHandle(), 0);

    if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
    {
        FATAL("Failed to create the framebuffer. Status :- " + std::to_string(glCheckFramebufferStatus(GL_FRAMEBUFFER)));
    }

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void Framebuffer::Destroy()
{
    m_DepthAttach.Destroy();
    m_ColAttach.Destroy();
    glDeleteFramebuffers(1, &m_Handle);   
}

void Framebuffer::Resize(int32_t width, int32_t height)
{
    if(m_ColAttach.GetWidth() == width && m_ColAttach.GetHeight() == height)
        return;

    if(width == 0 || height == 0)
        return;

    Destroy();

    Init(width, height);
}

void Framebuffer::Bind()
{
    glBindFramebuffer(GL_FRAMEBUFFER, m_Handle);
    m_ColAttach.Bind();
}

void Framebuffer::Unbind()
{
    m_ColAttach.Unbind();
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}