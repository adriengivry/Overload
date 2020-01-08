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
	* Wraps OpenGL VBO
	*/
	template <class T>
	class VertexBuffer
	{
	public:
		/**
		* Create the VBO using a pointer to the first element and a size (number of elements)
		* @param p_data
		* @parma p_elements
		*/
		VertexBuffer(T* p_data, size_t p_elements);

		/**
		* Create the EBO using a vector
		* @param p_data
		*/
		VertexBuffer(std::vector<T>& p_data);

		/**
		* Destructor
		*/
		~VertexBuffer();

		/**
		* Bind the buffer
		*/
		void Bind();

		/**
		* Bind the buffer
		*/
		void Unbind();

		/**
		* Returnd the ID of the VBO
		*/
		uint32_t GetID();

	private:
		uint32_t m_bufferID;
	};
}

#include "OvRendering/Buffers/VertexBuffer.inl"