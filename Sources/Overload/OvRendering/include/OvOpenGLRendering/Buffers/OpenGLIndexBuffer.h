#pragma once
#include "OvRendering/Buffers/IndexBuffer.h"

namespace OvRendering::OpenGL::Buffers
{
	/**
	* Wraps OpenGL EBO
	*/
	class OpenGLIndexBuffer
		:public OvRendering::Buffers::IndexBuffer
	{
	public:
		/**
		* Create the EBO using a pointer to the first element and a size (number of elements)
		* @param p_data
		* @parma p_elements
		*/
		OpenGLIndexBuffer(unsigned int* p_data, size_t p_elements);

		/**
		* Create the EBO using a vector
		* @param p_data
		*/
		OpenGLIndexBuffer(std::vector<uint32_t>& p_data);

		/**
		* Destructor
		*/
		~OpenGLIndexBuffer();

		/**
		* Bind the buffer
		*/
		virtual void Bind() override;

		/**
		* Unbind the buffer
		*/
		virtual void Unbind() override;

		/**
		* Returns the ID of the OpenGL EBO
		*/
		virtual uint32_t GetID() override;

	private:
		uint32_t m_bufferID;
	};

}