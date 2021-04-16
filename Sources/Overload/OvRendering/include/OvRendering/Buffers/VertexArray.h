/**
* @project: Overload
* @author: Overload Tech.
* @licence: MIT
*/

#pragma once

#include "OvRendering/Buffers/VertexBuffer.h"

namespace OvRendering::Buffers
{
	/**
	* Wraps from OpenGL types enum
	*/
	enum class EType
	{
		BYTE			= 0x1400,
		UNISGNED_BYTE	= 0x1401,
		SHORT			= 0x1402,
		UNSIGNED_SHORT	= 0x1403,
		INT				= 0x1404,
		UNSIGNED_INT	= 0x1405,
		FLOAT			= 0x1406,
		DOUBLE			= 0x140A
	};

	/**
	* Wraps OpenGL VAO
	*/
	class VertexArray
	{
	public:
		/**
		* Create the vertex array
		*/
		VertexArray();

		/**
		* Destroy the vertex array
		*/
		~VertexArray();

		/**
		* Register a VBO into the VAO
		* @param p_attribute
		* @param p_vertexBuffer
		* @param p_type
		* @param p_count
		* @param p_stride
		* @param p_offset
		*/
		template <class T>
		void BindAttribute(uint32_t p_attribute, VertexBuffer<T>& p_vertexBuffer, EType p_type, uint64_t p_count, uint64_t p_stride, intptr_t p_offset);

		/**
		* Bind the buffer
		*/
		void Bind();

		/**
		* Unbind the buffer
		*/
		void Unbind();

		/**
		* Return the VAO OpenGL ID
		*/
		GLint GetID();

	private:
		GLuint m_bufferID;
	};
}

#include "OvRendering/Buffers/VertexArray.inl"