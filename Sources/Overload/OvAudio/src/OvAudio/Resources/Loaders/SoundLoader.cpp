/**
* @project: Overload
* @author: Overload Tech.
* @licence: MIT
*/

#include "OvAudio/Resources/Loaders/SoundLoader.h"

OvAudio::Resources::Sound* OvAudio::Resources::Loaders::SoundLoader::Create(const std::string& p_filepath)
{
	return new Sound(p_filepath);
}

void OvAudio::Resources::Loaders::SoundLoader::Reload(Sound& p_sound, const std::string& p_path)
{
	*const_cast<std::string*>(&p_sound.path) = p_path;
}

bool OvAudio::Resources::Loaders::SoundLoader::Destroy(Sound*& p_soundInstance)
{
	if (p_soundInstance)
	{
		delete p_soundInstance;
		p_soundInstance = nullptr;

		return true;
	}

	return false;
}
