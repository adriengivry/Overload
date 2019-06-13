/**
* @project: Overload
* @author: Overload Tech.
* @restrictions: This software may not be resold, redistributed or otherwise conveyed to a third party.
*/

#pragma once

#include <OvAudio/Resources/Loaders/SoundLoader.h>

#include "OvCore/ResourceManagement/AResourceManager.h"

namespace OvCore::ResourceManagement
{
	/**
	* ResourceManager of sounds
	*/
	class API_OVCORE SoundManager : public AResourceManager<OvAudio::Resources::Sound>
	{
	public:
		/**
		* Create the resource identified by the given path
		* @param p_path
		*/
		virtual OvAudio::Resources::Sound* CreateResource(const std::string& p_path) override;

		/**
		* Destroy the given resource
		* @param p_resource
		*/
		virtual void DestroyResource(OvAudio::Resources::Sound* p_resource) override;
	};
}