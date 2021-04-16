/**
* @project: Overload
* @author: Overload Tech.
* @licence: MIT
*/

#pragma once

#include <vector>
#include <string>

#include "OvRendering/Context/Driver.h"
#include "OvRendering/Buffers/EAccessSpecifier.h"

namespace OvRendering::Resources { class Shader; }

namespace OvRendering::Buffers
{
	/**
	* Wraps OpenGL UBO
	*/
	class UniformBuffer
	{
	public:
		/**
		* Create a UniformBuffer
		* @param p_size (Specify the size in bytes of the UBO data)
		* @param p_bindingPoint (Specify the binding point on which the uniform buffer should be binded)
		* @parma p_offset (The offset of the UBO, sizeof previouses UBO if the binding point is != 0)
		* @param p_accessSpecifier
		*/
		UniformBuffer(size_t p_size, uint32_t p_bindingPoint = 0, uint32_t p_offset = 0, EAccessSpecifier p_accessSpecifier = EAccessSpecifier::DYNAMIC_DRAW);

		/**
		* Destructor of the UniformBuffer
		*/
		~UniformBuffer();

		/**
		* Bind the UBO
		*/
		void Bind();

		/**
		* Unbind the UBO
		*/
		void Unbind();

		/**
		* Set the data in the UBO located at p_offset to p_data
		* @param p_data
		* @param p_offset
		*/
		template<typename T>
		void SetSubData(const T& p_data, size_t p_offset);

		/**
		* Set the data in the UBO located at p_offset to p_data
		* @param p_data
		* @param p_offsetInOut (Will keep track of the current stride of the data layout)
		*/
		template<typename T>
		void SetSubData(const T& p_data, std::reference_wrapper<size_t> p_offsetInOut);

		/**
		* Return the ID of the UBO
		*/
		uint32_t GetID() const;

		/**
		* Bind a block identified by the given ID to given shader
		* @param p_shader
		* @param p_uniformBlockLocation
		* @param p_bindingPoint
		*/
		static void BindBlockToShader(OvRendering::Resources::Shader& p_shader, uint32_t p_uniformBlockLocation, uint32_t p_bindingPoint = 0);

		/**
		* Bind a block identified by the given name to the given shader
		* @param p_shader
		* @param p_name
		* @param p_bindingPoint
		*/
		static void BindBlockToShader(OvRendering::Resources::Shader& p_shader, const std::string& p_name, uint32_t p_bindingPoint = 0);

		/**
		* Return the location of the block (ID)
		* @param p_shader
		* @param p_name
		*/
		static uint32_t GetBlockLocation(OvRendering::Resources::Shader& p_shader, const std::string& p_name);

	private:
		uint32_t m_bufferID;
	};
}

#include "OvRendering/Buffers/UniformBuffer.inl"