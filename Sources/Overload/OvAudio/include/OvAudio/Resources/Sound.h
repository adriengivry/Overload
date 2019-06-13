/**
* @project: Overload
* @author: Overload Tech.
* @restrictions: This software may not be resold, redistributed or otherwise conveyed to a third party.
*/

#pragma once

#include <string>

#include "OvAudio/API/Export.h"

namespace OvAudio::Resources
{
	namespace Loaders { class SoundLoader; }

	/**
	* Playable sound
	*/
	class API_OVAUDIO Sound
	{
		friend class Loaders::SoundLoader;

	private:
		Sound(const std::string& p_path);

	public:
		const std::string path;
	};
}