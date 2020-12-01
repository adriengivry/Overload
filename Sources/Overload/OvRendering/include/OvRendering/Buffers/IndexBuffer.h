/**
* @project: Overload
* @author: Overload Tech.
* @licence: MIT
*/

#pragma once

#include <vector>

#include "OvRendering/Context/Driver.h"

namespace OvRendering::Buffers
{
	/**
	* Wraps OpenGL EBO
	*/
	class IndexBuffer
	{
	public:
		/**
		* Create the EBO using a pointer to the first element and a size (number of elements)
		* @param p_data
		* @parma p_elements
		*/
		IndexBuffer(unsigned int* p_data, size_t p_elements);

		/**
		* Create the EBO using a vector
		* @param p_data
		*/
		IndexBuffer(std::vector<uint32_t>& p_data);

		/**
		* Destructor
		*/
		~IndexBuffer();

		/**
		* Bind the buffer
		*/
		void Bind();

		/**
		* Unbind the buffer
		*/
		void Unbind();

		/**
		* Returns the ID of the OpenGL EBO
		*/
		uint32_t GetID();

	private:
		uint32_t m_bufferID;
	};
}