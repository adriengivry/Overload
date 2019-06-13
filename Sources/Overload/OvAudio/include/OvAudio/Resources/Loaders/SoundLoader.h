/**
* @project: Overload
* @author: Overload Tech.
* @restrictions: This software may not be resold, redistributed or otherwise conveyed to a third party.
*/

#pragma once

#include <string>

#include "OvAudio/Resources/Sound.h"

namespace OvAudio::Resources::Loaders
{
	/**
	* Handle the Mesh creation and destruction
	*/
	class API_OVAUDIO SoundLoader
	{
	public:
		/**
		* Disabled constructor
		*/
		SoundLoader() = delete;

		/**
		* Create a sound
		* @param p_filepath
		*/
		static Sound* Create(const std::string& p_filepath);

		/**
		* Destroy a sound
		* @param p_soundInstance
		*/
		static bool Destroy(Sound*& p_soundInstance);
	};
}