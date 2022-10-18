/**
* @project: Overload
* @author: Overload Tech.
* @licence: MIT
*/

#include <GL/glew.h>

#include "OvRendering/Buffers/IndexBuffer.h"
#include "OvOpenGLRendering/Buffers/OpenGLIndexBuffer.h"

namespace OvRendering::OpenGL::Buffers {


	OpenGLIndexBuffer::OpenGLIndexBuffer(unsigned int* p_data, size_t p_elements)
	{
		glGenBuffers(1, &m_bufferID);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_bufferID);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, p_elements * sizeof(unsigned int), p_data, GL_STATIC_DRAW);
	}

	OpenGLIndexBuffer::OpenGLIndexBuffer(std::vector<uint32_t>& p_data) : OpenGLIndexBuffer(p_data.data(), p_data.size())
	{
	}

	OpenGLIndexBuffer::~OpenGLIndexBuffer()
	{
		glDeleteBuffers(1, &m_bufferID);
	}

	void OpenGLIndexBuffer::Bind()
	{
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_bufferID);
	}

	void OpenGLIndexBuffer::Unbind()
	{
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	}

	uint32_t OpenGLIndexBuffer::GetID()
	{
		return m_bufferID;
	}

}