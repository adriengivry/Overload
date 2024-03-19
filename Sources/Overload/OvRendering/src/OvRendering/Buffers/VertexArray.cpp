/**
* @project: Overload
* @author: Overload Tech.
* @licence: MIT
*/

#include "OvRendering/Buffers/VertexArray.h"

OvRendering::Buffers::VertexArray::VertexArray()
{
	glGenVertexArrays(1, &m_bufferID);
	glBindVertexArray(m_bufferID);
}

OvRendering::Buffers::VertexArray::~VertexArray()
{
	glDeleteVertexArrays(1, &m_bufferID);
}

void OvRendering::Buffers::VertexArray::Bind() const
{
	glBindVertexArray(m_bufferID);
}

void OvRendering::Buffers::VertexArray::Unbind() const
{
	glBindVertexArray(0);
}

GLint OvRendering::Buffers::VertexArray::GetID() const
{
	return m_bufferID;
}
