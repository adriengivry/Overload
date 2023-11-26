/**
* @project: Overload
* @author: Overload Tech.
* @licence: MIT
*/

#pragma once

#include <string>

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
#include "OvRendering/Data/StateMask.h"

namespace OvRendering::Context
{
	/**
	* The Driver represents the OpenGL context
	*/
	class Driver
	{
	public:
		/**
		* Creates the Driver (OpenGL context)
		* @param p_driverSettings
		*/
		Driver(const Settings::DriverSettings& p_driverSettings);

		/**
		* Destroy the driver
		*/
		~Driver() = default;

		/**
		* Returns true if the OpenGL context is active
		*/
		bool IsActive() const;

		/**
		* Define a clear color. This color will be use for the next Renderer::Clear() call
		* @param p_red
		* @param p_green
		* @param p_blue
		* @param p_alpha
		*/
		void SetClearColor(float p_red, float p_green, float p_blue, float p_alpha = 1.0f) const;

		/**
		* Clear the screen using the previously defined clear color (With Renderer::SetClearColor()) or by
		* using the OpenGL default one.
		* @param p_colorBuffer
		* @param p_depthBuffer
		* @param p_stencilBuffer
		*/
		void Clear(bool p_colorBuffer = true, bool p_depthBuffer = true, bool p_stencilBuffer = true) const;

		/**
		* Defines the rasterization lines width
		* @param p_width
		*/
		void SetRasterizationLinesWidth(float p_width) const;

		/**
		* Defines the rasterization mode to use
		* @param p_rasterizationMode
		*/
		void SetRasterizationMode(Settings::ERasterizationMode p_rasterizationMode) const;

		/**
		* Enable or disable an OpenGL capability
		* @param p_capability
		* @param p_value
		*/
		void SetCapability(Settings::ERenderingCapability p_capability, bool p_value) const;

		/**
		* Returns true if the given OpenGL capability is enabled
		* @param p_capability
		*/
		bool GetCapability(Settings::ERenderingCapability p_capability) const;

		/**
		* Defines the comparaison algorithm to use for the stencil buffer test
		* @param p_algorithm
		* @param p_reference
		* @param p_mask
		*/
		void SetStencilAlgorithm(Settings::EComparaisonAlgorithm p_algorithm, int32_t p_reference, uint32_t p_mask) const;

		/**
		* Defines the comparaison algorithm to use for the depth buffer test
		* @param p_algorithm
		*/
		void SetDepthAlgorithm(Settings::EComparaisonAlgorithm p_algorithm) const;

		/**
		* Control the front and back writing of individual bits in the stencil planes
		* @parma p_mask
		*/
		void SetStencilMask(uint32_t p_mask) const;

		/**
		* Defines the stencil test operations
		* @param p_stencilFail : Specifies the action to take when the stencil test fails
		* @param p_depthFail : Specifies the stencil action when the stencil test passes, but the depth test fails.
		* @param p_bothPass : Specifies the stencil action when both the stencil test and the depth test pass, or when the stencil test passes and either there is no depth buffer or depth testing is not enabled.
		*/
		void SetStencilOperations(Settings::EOperation p_stencilFail = Settings::EOperation::KEEP, Settings::EOperation p_depthFail = Settings::EOperation::KEEP, Settings::EOperation p_bothPass = Settings::EOperation::KEEP) const;

		/**
		* Defines the culling faces
		* @param p_cullface
		*/
		void SetCullFace(Settings::ECullFace p_cullFace) const;

		/**
		* Enable or disable writing into the depth buffer
		* @param p_enable
		*/
		void SetDepthWriting(bool p_enable) const;

		/**
		* Enable and disable writing color individual components into the frame buffer
		* @param p_enableRed
		* @param p_enableGreen
		* @param p_enableBlue
		* @param p_enableAlpha
		*/
		void SetColorWriting(bool p_enableRed, bool p_enableGreen, bool p_enableBlue, bool p_enableAlpha) const;

		/**
		* Enable and disable writing color into the frame buffer (Equivalent to Renderer::SetColorWriting(p_enable, p_enable, p_enable, p_enable))
		* @param p_enable
		*/
		void SetColorWriting(bool p_enable) const;

		/**
		 * Set the viewport parameters.
		 * @param x
		 * @param y
		 * @param width
		 * @param height
		 */
		void SetViewPort(uint32_t x, uint32_t y, uint32_t width, uint32_t height) const;

		/**
		 * Read a block of pixels from the frame buffer.
		 * @param x
		 * @param y
		 * @param width
		 * @param height
		 * @param format
		 * @param type
		 * @param data
		 */
		void ReadPixels(uint32_t x, uint32_t y, uint32_t width, uint32_t height, Settings::EPixelDataFormat format, Settings::EPixelDataType type, void* data) const;

		/**
		* Return the value associated to the given GLenum
		* @param p_parameter
		*/
		bool GetBool(uint32_t p_parameter) const;

		/**
		* Return the value associated to the given GLenum at the given index.
		* @param p_parameter
		* @param p_index
		*/
		bool GetBool(uint32_t p_parameter, uint32_t p_index) const;

		/**
		* Return the value associated to the given GLenum
		* @param p_parameter
		*/
		int GetInt(uint32_t p_parameter) const;

		/**
		* Return the value associated to the given GLenum at the given index.
		* @param p_parameter
		* @param p_index
		*/
		int GetInt(uint32_t p_parameter, uint32_t p_index) const;

		/**
		* Return the value associated to the given GLenum
		* @param p_parameter
		*/
		float GetFloat(uint32_t p_parameter) const;

		/**
		* Return the value associated to the given GLenum at the given index.
		* @param p_parameter
		* @param p_index
		*/
		float GetFloat(uint32_t p_parameter, uint32_t p_index) const;

		/**
		* Return the value associated to the given GLenum
		* @param p_parameter
		*/
		double GetDouble(uint32_t p_parameter) const;

		/**
		* Return the value associated to the given GLenum at the given index.
		* @param p_parameter
		* @param p_index
		*/
		double GetDouble(uint32_t p_parameter, uint32_t p_index) const;

		/**
		* Return the value associated to the given GLenum
		* @param p_parameter
		*/
		int64_t GetInt64(uint32_t p_parameter) const;

		/**
		* Return the value associated to the given GLenum at the given index.
		* @param p_parameter
		* @param p_index
		*/
		int64_t GetInt64(uint32_t p_parameter, uint32_t p_index) const;

		/**
		* Return the value associated to the given GLenum
		* @param p_parameter
		*/
		std::string GetString(uint32_t p_parameter) const;

		/**
		* Return the value associated to the given GLenum at the given index.
		* @param p_parameter
		* @param p_index
		*/
		std::string GetString(uint32_t p_parameter, uint32_t p_index) const;

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
		* Update the current state mask by fetching data from the driver
		* @note Slow! Avoid using (although necessary when the driver got modified directly)
		*/
		void UpdateStateMask();

		/**
		* Fetch driver state to return state mask
		*/
		const Data::StateMask GetStateMask() const;

		/**
		* Apply state mask to modify current driver state
		* @param p_mask
		*/
		void ApplyStateMask(Data::StateMask p_mask);

	private:
		Data::StateMask m_stateMask;
		bool m_isActive;
	};
}