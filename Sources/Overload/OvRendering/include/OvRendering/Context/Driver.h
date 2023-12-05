/**
* @project: Overload
* @author: Overload Tech.
* @licence: MIT
*/

#pragma once

#include <string>
#include <array>

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
#include "OvRendering/Data/PipelineState.h"

#include <OvMaths/FVector4.h>
#include <OvTools/Utils/OptRef.h>

namespace OvRendering::Context
{
	/**
	* Handles the lifecycle of the underlying graphics context
	*/
	class Driver
	{
	public:
		/**
		* Creates the driver
		* @param p_driverSettings
		*/
		Driver(const Settings::DriverSettings& p_driverSettings);

		/**
		* Destroy the driver
		*/
		~Driver() = default;

		/**
		* Set the viewport
		* @param p_x
		* @param p_y
		* @param p_width
		* @param p_height
		*/
		void SetViewport(
			uint32_t p_x,
			uint32_t p_y,
			uint32_t p_width,
			uint32_t p_height
		);

		/**
		* Clear the screen using the previously defined clear color (With Renderer::SetClearColor()) or by
		* using the OpenGL default one.
		* @param p_colorBuffer
		* @param p_depthBuffer
		* @param p_stencilBuffer
		* @param p_color
		*/
		void Clear(
			bool p_colorBuffer,
			bool p_depthBuffer,
			bool p_stencilBuffer,
			OvTools::Utils::OptRef<const OvMaths::FVector4> p_color
		) const;

		/**
		 * Read a block of pixels from the currently bound framebuffer (or backbuffer).
		 * @param p_x
		 * @param p_y
		 * @param p_width
		 * @param p_height
		 * @param p_format
		 * @param p_type
		 * @param p_data
		 */
		void ReadPixels(
			uint32_t p_x,
			uint32_t p_y,
			uint32_t p_width,
			uint32_t p_height,
			Settings::EPixelDataFormat p_format,
			Settings::EPixelDataType p_type,
			void* p_data
		) const;

		/**
		* Issue a draw call command (EBO) using the given primitive mode
		* @param p_primitiveMode
		* @param p_indexCount
		*/
		void DrawElements(Settings::EPrimitiveMode p_primitiveMode, uint32_t p_indexCount) const;

		/**
		* Issue an instanced draw call (EBO) command using the given primitive mode
		* @param p_primitiveMode
		* @param p_indexCount
		* @param p_instances
		*/
		void DrawElementsInstanced(Settings::EPrimitiveMode p_primitiveMode, uint32_t p_indexCount, uint32_t p_instances) const;

		/**
		* Issue a draw call command (VBO) using the given primitive mode
		* @param p_primitiveMode
		* @param p_indexCount
		* @note Should be avoided, prefer using DrawElements
		*/
		void DrawArrays(Settings::EPrimitiveMode p_primitiveMode, uint32_t p_vertexCount) const;

		/**
		* Issue an instanced draw call (VBO) command using the given primitive mode
		* @param p_primitiveMode
		* @param p_vertexCount
		* @param p_instances
		* @note Should be avoided, prefer using DrawElements
		*/
		void DrawArraysInstanced(Settings::EPrimitiveMode p_primitiveMode, uint32_t p_vertexCount, uint32_t p_instances) const;

		/**
		* Set the pipeline state to match the given PSO
		* @param p_state
		*/
		void SetPipelineState(Data::PipelineState p_state);

		/**
		* Reset the pipeline state to its default state
		* @param p_state
		*/
		void ResetPipelineState();

		/**
		* Return the current pipeline state
		*/
		const Data::PipelineState& GetPipelineState() const;

		/**
		* Create a pipeline state from the default state
		*/
		Data::PipelineState CreatePipelineState() const;

		/**
		* Returns the vendor
		*/
		std::string_view GetVendor() const;

		/**
		* Returns details about the current rendering hardware
		*/
		std::string_view GetHardware() const;

		/**
		* Returns the current graphics API version
		*/
		std::string_view GetVersion() const;

		/**
		* Returns the current shading language version
		*/
		std::string_view GetShadingLanguageVersion() const;

	private:
		std::string m_vendor;
		std::string m_hardware;
		std::string m_version;
		std::string m_shadingLanguageVersion;

		Data::PipelineState m_defaultPipelineState;
		Data::PipelineState m_pipelineState;
	};
}