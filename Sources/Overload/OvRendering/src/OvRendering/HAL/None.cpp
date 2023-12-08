/**
* @project: Overload
* @author: Overload Tech.
* @licence: MIT
*/

#pragma once

#include "OvRendering/HAL/GraphicsAPI.h"

/**
* Although this is intended as a Null implementation, certain components of the engine rely on OpenGL,
* such as resource creation, binding, and the user interface. Consequently, GLEW must be initialized to
* support these functionalities. This implementation exclusively initializes GLEW without making any
* additional calls.
*/
#include <GL/glew.h>

namespace OvRendering::HAL
{
	std::optional<OvRendering::Data::PipelineState> None::Init(bool debug)
	{
		glewInit(); // <-- initialize GLEW; see comment above the GLEW include directive for more details.
		return OvRendering::Data::PipelineState{};
	}

	void None::Clear(bool p_colorBuffer, bool p_depthBuffer, bool p_stencilBuffer)
	{}

	void None::ReadPixels(
		uint32_t p_x,
		uint32_t p_y,
		uint32_t p_width,
		uint32_t p_height,
		Settings::EPixelDataFormat p_format,
		Settings::EPixelDataType p_type,
		void* p_data
	)
	{}

	void None::DrawElements(Settings::EPrimitiveMode p_primitiveMode, uint32_t p_indexCount)
	{}

	void None::DrawElementsInstanced(Settings::EPrimitiveMode p_primitiveMode, uint32_t p_indexCount, uint32_t p_instances)
	{}

	void None::DrawArrays(Settings::EPrimitiveMode p_primitiveMode, uint32_t p_vertexCount)
	{}

	void None::DrawArraysInstanced(Settings::EPrimitiveMode p_primitiveMode, uint32_t p_vertexCount, uint32_t p_instances)
	{}

	void None::SetClearColor(float p_red, float p_green, float p_blue, float p_alpha)
	{}

	void None::SetRasterizationLinesWidth(float p_width)
	{}

	void None::SetRasterizationMode(Settings::ERasterizationMode p_rasterizationMode)
	{}

	void None::SetCapability(Settings::ERenderingCapability p_capability, bool p_value)
	{}

	bool None::GetCapability(Settings::ERenderingCapability p_capability)
	{
		return false;
	}

	void None::SetStencilAlgorithm(Settings::EComparaisonAlgorithm p_algorithm, int32_t p_reference, uint32_t p_mask)
	{}

	void None::SetDepthAlgorithm(Settings::EComparaisonAlgorithm p_algorithm)
	{}

	void None::SetStencilMask(uint32_t p_mask)
	{}

	void None::SetStencilOperations(Settings::EOperation p_stencilFail, Settings::EOperation p_depthFail, Settings::EOperation p_bothPass)
	{}

	void None::SetCullFace(Settings::ECullFace p_cullFace)
	{}

	void None::SetDepthWriting(bool p_enable)
	{}

	void None::SetColorWriting(bool p_enableRed, bool p_enableGreen, bool p_enableBlue, bool p_enableAlpha)
	{}

	void None::SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height)
	{}

	std::string None::GetVendor()
	{
		return "None";
	}

	std::string None::GetHardware()
	{
		return "None";
	}

	std::string None::GetVersion()
	{
		return "None";
	}

	std::string None::GetShadingLanguageVersion()
	{
		return "None";
	}
}
