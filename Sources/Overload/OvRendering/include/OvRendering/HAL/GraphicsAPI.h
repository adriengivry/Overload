/**
* @project: Overload
* @author: Overload Tech.
* @licence: MIT
*/

#pragma once

#include "OvRendering/Settings/DriverSettings.h"
#include "OvRendering/Settings/ERenderingCapability.h"
#include "OvRendering/Settings/EPrimitiveMode.h"
#include "OvRendering/Settings/ERasterizationMode.h"
#include "OvRendering/Settings/EComparaisonAlgorithm.h"
#include "OvRendering/Settings/EOperation.h"
#include "OvRendering/Settings/ECullFace.h"
#include "OvRendering/Settings/ECullingOptions.h"
#include "OvRendering/Settings/EPixelDataFormat.h"
#include "OvRendering/Settings/EPixelDataType.h"
#include "OvRendering/Settings/EGraphicsBackend.h"
#include "OvRendering/Data/PipelineState.h"

namespace OvRendering::HAL
{
	template<Settings::EGraphicsBackend Backend>
	class GraphicsAPI final
	{
	public:
		std::optional<OvRendering::Data::PipelineState> Init(bool debug);

		void Clear(bool p_colorBuffer, bool p_depthBuffer, bool p_stencilBuffer);
		void ReadPixels(
			uint32_t p_x,
			uint32_t p_y,
			uint32_t p_width,
			uint32_t p_height,
			OvRendering::Settings::EPixelDataFormat p_format,
			OvRendering::Settings::EPixelDataType p_type,
			void* p_data
		);

		void DrawElements(OvRendering::Settings::EPrimitiveMode p_primitiveMode, uint32_t p_indexCount);
		void DrawElementsInstanced(OvRendering::Settings::EPrimitiveMode p_primitiveMode, uint32_t p_indexCount, uint32_t p_instances);
		void DrawArrays(OvRendering::Settings::EPrimitiveMode p_primitiveMode, uint32_t p_vertexCount);
		void DrawArraysInstanced(OvRendering::Settings::EPrimitiveMode p_primitiveMode, uint32_t p_vertexCount, uint32_t p_instances);

		void SetClearColor(float p_red, float p_green, float p_blue, float p_alpha);
		void SetRasterizationLinesWidth(float p_width);
		void SetRasterizationMode(OvRendering::Settings::ERasterizationMode p_rasterizationMode);
		void SetCapability(OvRendering::Settings::ERenderingCapability p_capability, bool p_value);
		bool GetCapability(OvRendering::Settings::ERenderingCapability p_capability);
		void SetStencilAlgorithm(OvRendering::Settings::EComparaisonAlgorithm p_algorithm, int32_t p_reference, uint32_t p_mask);
		void SetDepthAlgorithm(OvRendering::Settings::EComparaisonAlgorithm p_algorithm);
		void SetStencilMask(uint32_t p_mask);

		void SetStencilOperations(
			OvRendering::Settings::EOperation p_stencilFail,
			OvRendering::Settings::EOperation p_depthFail,
			OvRendering::Settings::EOperation p_bothPass
		);

		void SetCullFace(OvRendering::Settings::ECullFace p_cullFace);
		void SetDepthWriting(bool p_enable);
		void SetColorWriting(bool p_enableRed, bool p_enableGreen, bool p_enableBlue, bool p_enableAlpha);
		void SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height);

		std::string GetVendor();
		std::string GetHardware();
		std::string GetVersion();
		std::string GetShadingLanguageVersion();
	};

	using Null = OvRendering::HAL::GraphicsAPI<OvRendering::Settings::EGraphicsBackend::NONE>;
	using OpenGL = OvRendering::HAL::GraphicsAPI<OvRendering::Settings::EGraphicsBackend::OPENGL>;
}
