/**
* @project: Overload
* @author: Overload Tech.
* @licence: MIT
*/

#include "OvCore/ResourceManagement/MaterialManager.h"

OvCore::Resources::Material * OvCore::ResourceManagement::MaterialManager::CreateResource(const std::string & p_path)
{
	std::string realPath = GetRealPath(p_path);

	Resources::Material* material = OvCore::Resources::Loaders::MaterialLoader::Create(realPath);
	if (material)
	{
		*reinterpret_cast<std::string*>(reinterpret_cast<char*>(material) + offsetof(Resources::Material, path)) = p_path; // Force the resource path to fit the given path
	}

	return material;
}

void OvCore::ResourceManagement::MaterialManager::DestroyResource(OvCore::Resources::Material * p_resource)
{
	OvCore::Resources::Loaders::MaterialLoader::Destroy(p_resource);
}

void OvCore::ResourceManagement::MaterialManager::ReloadResource(OvCore::Resources::Material* p_resource, const std::string& p_path)
{
	OvCore::Resources::Loaders::MaterialLoader::Reload(*p_resource, p_path);
}
