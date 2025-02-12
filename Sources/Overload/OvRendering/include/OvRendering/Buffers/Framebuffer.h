/**
* @project: Overload
* @author: Overload Tech.
* @licence: MIT
*/

#pragma once

#include <vector>

#include "OvRendering/Context/Driver.h"
#include "OvRendering/Resources/Texture.h"

namespace OvRendering::Buffers
{
	/**
	* Wraps OpenGL EBO
	*/
	class Framebuffer
	{
	public:
		/**
		* Create the framebuffer
		* @param p_width
		* @param p_height
		* @param p_depthOnly
		*/
		Framebuffer(uint16_t p_width = 0, uint16_t p_height = 0, bool p_depthOnly = false);

		/**
		* Destructor
		*/
		~Framebuffer();

		/**
		* Bind the framebuffer
		*/
		void Bind() const;

		/**
		* Unbind the framebuffer
		*/
		void Unbind() const;

		/**
		* Defines a new size for the framebuffer
		* @param p_width
		* @param p_height
		* @param p_forceUpdate Force the resizing operation even if the width and height didn't change
		*/
		void Resize(uint16_t p_width, uint16_t p_height, bool p_forceUpdate = false);

		/**
		* Returns the ID of the OpenGL framebuffer
		*/
		uint32_t GetID() const;

		/**
		* Returns the ID of the OpenGL render texture
		*/
		uint32_t GetTextureID() const;

		/**
		* Returns the ID of the OpenGL render texture
		*/
		Resources::TextureHandle GetTexture() const;

		/**
		* Returns the ID of the OpenGL render buffer
		*/
		uint32_t GetRenderBufferID() const;

		/**
		* Returns the width of the framebuffer
		*/
		uint16_t GetWidth() const;

		/**
		* Returns the width of the framebuffer
		*/
		uint16_t GetHeight() const;

		/**
		* Generate mip maps for the framebuffer's associated render texture
		*/
		void GenerateMipMaps() const;

		/**
		* Blit the framebuffer to the back buffer
		* @param p_backBufferWidth
		* @param p_backBufferHeight
		*/
		void BlitToBackBuffer(uint16_t p_backBufferWidth, uint16_t p_backBufferHeight) const;

	private:
		uint16_t m_width = 0;
		uint16_t m_height = 0;
		bool m_depthOnly = false;

		uint32_t m_bufferID = 0;
		uint32_t m_renderTexture = 0;
		uint32_t m_depthStencilBuffer = 0;
	};
}