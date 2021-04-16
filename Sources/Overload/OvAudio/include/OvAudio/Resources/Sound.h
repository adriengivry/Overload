/**
* @project: Overload
* @author: Overload Tech.
* @licence: MIT
*/

#pragma once

#include <string>



namespace OvAudio::Resources
{
	namespace Loaders { class SoundLoader; }

	/**
	* Playable sound
	*/
	class Sound
	{
		friend class Loaders::SoundLoader;

	private:
		Sound(const std::string& p_path);

	public:
		const std::string path;
	};
}