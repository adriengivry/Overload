/**
* @project: Overload
* @author: Overload Tech.
* @licence: MIT
*/

#include "OvCore/ResourceManagement/SoundManager.h"

OvAudio::Resources::Sound* OvCore::ResourceManagement::SoundManager::CreateResource(const std::string& p_path)
{
	std::string realPath = GetRealPath(p_path);
	OvAudio::Resources::Sound* sound = OvAudio::Resources::Loaders::SoundLoader::Create(realPath);
	if (sound)
		*reinterpret_cast<std::string*>(reinterpret_cast<char*>(sound) + offsetof(OvAudio::Resources::Sound, path)) = p_path; // Force the resource path to fit the given path

	return sound;
}

void OvCore::ResourceManagement::SoundManager::DestroyResource(OvAudio::Resources::Sound* p_resource)
{
	OvAudio::Resources::Loaders::SoundLoader::Destroy(p_resource);
}

void OvCore::ResourceManagement::SoundManager::ReloadResource(OvAudio::Resources::Sound* p_resource, const std::string& p_path)
{
}
