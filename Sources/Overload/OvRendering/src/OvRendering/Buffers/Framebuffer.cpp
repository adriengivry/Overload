/**
* @project: Overload
* @author: Overload Tech.
* @licence: MIT
*/

#include "OvRendering/Buffers/Framebuffer.h"

#include <GL/glew.h>

#include <OvDebug/Assertion.h>
#include <iostream>

OvRendering::Buffers::Framebuffer::Framebuffer(uint16_t p_width, uint16_t p_height, bool p_depthOnly) :
	m_width(p_width),
	m_height(p_height),
	m_depthOnly(p_depthOnly)
{
	// Generate OpenGL objects
	glGenFramebuffers(1, &m_bufferID);
	glGenTextures(1, &m_renderTexture);
	if (!m_depthOnly)
	{
		glGenRenderbuffers(1, &m_depthStencilBuffer);
	}

	// Setup texture
	glBindTexture(GL_TEXTURE_2D, m_renderTexture);
	if (m_depthOnly)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, m_width, m_height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, nullptr);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	}
	else
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, m_width, m_height, 0, GL_RGB, GL_UNSIGNED_BYTE, nullptr);
	}

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	float borderColor[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);

	glBindTexture(GL_TEXTURE_2D, 0);

	// Setup framebuffer
	Bind();
	if (m_depthOnly)
	{
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, m_renderTexture, 0);
		glDrawBuffer(GL_NONE);
		glReadBuffer(GL_NONE);
	}
	else
	{
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_renderTexture, 0);

		// Setup depth-stencil buffer
		glBindRenderbuffer(GL_RENDERBUFFER, m_depthStencilBuffer);
		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_STENCIL, m_width, m_height);
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, m_depthStencilBuffer);
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_STENCIL_ATTACHMENT, GL_RENDERBUFFER, m_depthStencilBuffer);
		glBindRenderbuffer(GL_RENDERBUFFER, 0);
	}

	// OVASSERT(glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE, "Framebuffer is incomplete!");

	Unbind();
}

OvRendering::Buffers::Framebuffer::~Framebuffer()
{
	// Destroy OpenGL objects
	glDeleteFramebuffers(1, &m_bufferID);
	glDeleteTextures(1, &m_renderTexture);
	if (!m_depthOnly) {
		glDeleteRenderbuffers(1, &m_depthStencilBuffer);
	}
}

void OvRendering::Buffers::Framebuffer::Bind() const
{
	glBindFramebuffer(GL_FRAMEBUFFER, m_bufferID);
}

void OvRendering::Buffers::Framebuffer::Unbind() const
{
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void OvRendering::Buffers::Framebuffer::Resize(uint16_t p_width, uint16_t p_height, bool p_forceUpdate)
{
	if (p_forceUpdate || p_width != m_width || p_height != m_height)
	{
		m_width = p_width;
		m_height = p_height;

		// Resize texture
		glBindTexture(GL_TEXTURE_2D, m_renderTexture);
		if (m_depthOnly) {
			glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, m_width, m_height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, nullptr);
		}
		else {
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, m_width, m_height, 0, GL_RGB, GL_UNSIGNED_BYTE, nullptr);
		}
		glBindTexture(GL_TEXTURE_2D, 0);

		if (!m_depthOnly) {
			// Resize depth-stencil buffer
			glBindRenderbuffer(GL_RENDERBUFFER, m_depthStencilBuffer);
			glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_STENCIL, m_width, m_height);
			glBindRenderbuffer(GL_RENDERBUFFER, 0);
		}
	}
}

uint32_t OvRendering::Buffers::Framebuffer::GetID() const
{
	return m_bufferID;
}

uint32_t OvRendering::Buffers::Framebuffer::GetTextureID() const
{
	return m_renderTexture;
}

OvRendering::Resources::TextureHandle OvRendering::Buffers::Framebuffer::GetTexture() const
{
	return Resources::TextureHandle(m_renderTexture);
}

uint32_t OvRendering::Buffers::Framebuffer::GetRenderBufferID() const
{
	return m_depthStencilBuffer;
}
