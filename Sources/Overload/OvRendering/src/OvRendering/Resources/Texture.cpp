/**
* @project: Overload
* @author: Overload Tech.
* @licence: MIT
*/

#include <GL/glew.h>

#include "OvRendering/Resources/Texture.h"

void OvRendering::Resources::Texture::Bind(uint32_t p_slot) const
{
	glActiveTexture(GL_TEXTURE0 + p_slot);
	glBindTexture(GL_TEXTURE_2D, id);
}

void OvRendering::Resources::Texture::Unbind() const
{
	glBindTexture(GL_TEXTURE_2D, 0);
}

OvRendering::Resources::Texture::Texture(const std::string p_path, uint32_t p_id, uint32_t p_width, uint32_t p_height, uint32_t p_bpp, Settings::ETextureFilteringMode p_firstFilter, Settings::ETextureFilteringMode p_secondFilter, bool p_generateMipmap) : path(p_path),
	id(p_id), width(p_width), height(p_height), bitsPerPixel(p_bpp), firstFilter(p_firstFilter), secondFilter(p_secondFilter), isMimapped(p_generateMipmap)
{

}
