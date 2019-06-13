/**
* @project: Overload
* @author: Overload Tech.
* @restrictions: This software may not be resold, redistributed or otherwise conveyed to a third party.
*/

#include "OvCore/ResourceManagement/TextureManager.h"
#include "OvRendering/Settings/DriverSettings.h"

OvRendering::Resources::Texture* OvCore::ResourceManagement::TextureManager::CreateResource(const std::string & p_path)
{
	OvRendering::Settings::ETextureFilteringMode firstFilter = OvRendering::Settings::ETextureFilteringMode::LINEAR_MIPMAP_LINEAR;
	OvRendering::Settings::ETextureFilteringMode secondFilter = OvRendering::Settings::ETextureFilteringMode::LINEAR;

	std::string realPath = GetRealPath(p_path);
	OvRendering::Resources::Texture* texture = OvRendering::Resources::Loaders::TextureLoader::Create(realPath, firstFilter, secondFilter, true);
	if (texture)
		*reinterpret_cast<std::string*>(reinterpret_cast<char*>(texture) + offsetof(OvRendering::Resources::Texture, path)) = p_path; // Force the resource path to fit the given path

	return texture;
}

void OvCore::ResourceManagement::TextureManager::DestroyResource(OvRendering::Resources::Texture* p_resource)
{
	OvRendering::Resources::Loaders::TextureLoader::Destroy(p_resource);
}
