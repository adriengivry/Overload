/**
* @project: Overload
* @author: Overload Tech.
* @restrictions: This software may not be resold, redistributed or otherwise conveyed to a third party.
*/

#include "OvAudio/Resources/Loaders/SoundLoader.h"

OvAudio::Resources::Sound* OvAudio::Resources::Loaders::SoundLoader::Create(const std::string& p_filepath)
{
	return new Sound(p_filepath);
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
