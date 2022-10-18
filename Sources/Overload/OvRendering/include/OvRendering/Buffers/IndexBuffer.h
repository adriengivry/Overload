/**
* @project: Overload
* @author: Overload Tech.
* @licence: MIT
*/

#pragma once

#include <vector>
#include <memory>

#include "OvRendering/Context/Driver.h"

namespace OvRendering::Buffers
{
	/**
	* IndexBuffer abstraction layer
	*/
	class IndexBuffer
	{
	public:
		/**
		* Create the EBO using a pointer to the first element and a size (number of elements)
		* @param p_data
		* @parma p_elements
		*/
		static std::unique_ptr<IndexBuffer> Create(unsigned int* p_data, size_t p_elements);

		/**
		* Create the EBO using a vector
		* @param p_data
		*/
		inline static std::unique_ptr<IndexBuffer> Create(std::vector<uint32_t>& p_data)
		{
			return Create(p_data.data(), p_data.size());
		}

		/**
		* Bind the buffer
		*/
		virtual void Bind() = 0;

		/**
		* Unbind the buffer
		*/
		virtual void Unbind() = 0;

		/**
		* Returns the ID of the OpenGL EBO
		*/
		virtual uint32_t GetID() = 0;

	};
}