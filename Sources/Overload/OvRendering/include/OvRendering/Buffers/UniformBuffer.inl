/**
* @project: Overload
* @author: Overload Tech.
* @licence: MIT
*/

#pragma once

#include <GL/glew.h>

#include "OvRendering/Buffers/UniformBuffer.h"

namespace OvRendering::Buffers
{
	template<typename T>
	inline void UniformBuffer::SetSubData(const T& p_data, size_t p_offsetInOut)
	{
		Bind();
		glBufferSubData(GL_UNIFORM_BUFFER, p_offsetInOut, sizeof(T), std::addressof(p_data));
		Unbind();
	}

	template<typename T>
	inline void UniformBuffer::SetSubData(const T& p_data, std::reference_wrapper<size_t> p_offsetInOut)
	{
		Bind();
		size_t dataSize = sizeof(T);
		glBufferSubData(GL_UNIFORM_BUFFER, p_offsetInOut.get(), dataSize, std::addressof(p_data));
		p_offsetInOut.get() += dataSize;
		Unbind();
	}
}