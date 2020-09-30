/**
* @project: Overload
* @author: Overload Tech.
* @licence: MIT
*/

#include "OvCore/ResourceManagement/TextureManager.h"
#include "OvRendering/Settings/DriverSettings.h"

#include <OvTools/Filesystem/IniFile.h>

std::tuple<OvRendering::Settings::ETextureFilteringMode, OvRendering::Settings::ETextureFilteringMode, bool> GetAssetMetadata(const std::string& p_path)
{
	auto metaFile = OvTools::Filesystem::IniFile(p_path + ".meta");

	auto min = metaFile.GetOrDefault("MIN_FILTER", static_cast<int>(OvRendering::Settings::ETextureFilteringMode::LINEAR_MIPMAP_LINEAR));
	auto mag = metaFile.GetOrDefault("MAG_FILTER", static_cast<int>(OvRendering::Settings::ETextureFilteringMode::LINEAR));
	auto mipmap = metaFile.GetOrDefault("ENABLE_MIPMAPPING", true);

	return { static_cast<OvRendering::Settings::ETextureFilteringMode>(min), static_cast<OvRendering::Settings::ETextureFilteringMode>(mag), mipmap };
}

OvRendering::Resources::Texture* OvCore::ResourceManagement::TextureManager::CreateResource(const std::string & p_path)
{
	std::string realPath = GetRealPath(p_path);

	auto [min, mag, mipmap] = GetAssetMetadata(realPath);

	OvRendering::Resources::Texture* texture = OvRendering::Resources::Loaders::TextureLoader::Create(realPath, min, mag, mipmap);
	if (texture)
		*reinterpret_cast<std::string*>(reinterpret_cast<char*>(texture) + offsetof(OvRendering::Resources::Texture, path)) = p_path; // Force the resource path to fit the given path

	return texture;
}

void OvCore::ResourceManagement::TextureManager::DestroyResource(OvRendering::Resources::Texture* p_resource)
{
	OvRendering::Resources::Loaders::TextureLoader::Destroy(p_resource);
}

void OvCore::ResourceManagement::TextureManager::ReloadResource(OvRendering::Resources::Texture* p_resource, const std::string& p_path)
{
	std::string realPath = GetRealPath(p_path);

	auto [min, mag, mipmap] = GetAssetMetadata(realPath);

	OvRendering::Resources::Loaders::TextureLoader::Reload(*p_resource, realPath, min, mag, mipmap);
}
