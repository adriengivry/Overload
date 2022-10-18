#pragma once

#include <memory>
#include "OvOpenGLRendering/Buffers/OpenGLIndexBuffer.h"
#include "OvRendering/Settings/EActiveRenderingApi.h"
#include "OvDebug/Assertion.h"
#include "OvRendering/Core/Renderer.h"
#include "OvRendering/Settings/EActiveRenderingApi.h"

namespace OvRendering::Buffers
{


	std::unique_ptr<IndexBuffer> IndexBuffer::Create(unsigned int* p_data, size_t p_elements)
	{
		switch (Core::Renderer::GetActiveRenderingApi())
		{
		case  OvRendering::Settings::ERederingApi::OpenGL:
			return std::make_unique<OpenGL::Buffers::OpenGLIndexBuffer>(p_data, p_elements);
			break;

		default:
			break;
		}

		OvDebug::Assertion::Assert(false, "No valid rendering api selected");
		return nullptr;
	}

}