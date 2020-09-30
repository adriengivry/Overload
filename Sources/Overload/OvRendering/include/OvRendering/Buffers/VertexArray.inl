/**
* @project: Overload
* @author: Overload Tech.
* @licence: MIT
*/

#pragma once

#include <GL/glew.h>

#include "OvRendering/Buffers/VertexArray.h"

namespace OvRendering::Buffers
{
	template <class T>
	inline void VertexArray::BindAttribute(uint32_t p_attribute, VertexBuffer<T>& p_vertexBuffer, EType p_type, uint64_t p_count, uint64_t p_stride, intptr_t p_offset)
	{
		Bind();
		p_vertexBuffer.Bind();
		glEnableVertexAttribArray(p_attribute);
		glVertexAttribPointer(static_cast<GLuint>(p_attribute), static_cast<GLint>(p_count), static_cast<GLenum>(p_type), GL_FALSE, static_cast<GLsizei>(p_stride), reinterpret_cast<const GLvoid*>(p_offset));
	}
}