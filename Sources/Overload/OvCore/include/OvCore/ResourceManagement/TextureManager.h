/**
* @project: Overload
* @author: Overload Tech.
* @restrictions: This software may not be resold, redistributed or otherwise conveyed to a third party.
*/

#pragma once

#include <OvRendering/Resources/Loaders/TextureLoader.h>

#include "OvCore/ResourceManagement/AResourceManager.h"

namespace OvCore::ResourceManagement
{
	/**
	* ResourceManager of textures
	*/
	class API_OVCORE TextureManager : public AResourceManager<OvRendering::Resources::Texture>
	{
	public:
		/**
		* Create the resource identified by the given path
		* @param p_path
		*/
		virtual OvRendering::Resources::Texture* CreateResource(const std::string & p_path) override;

		/**
		* Destroy the given resource
		* @param p_resource
		*/
		virtual void DestroyResource(OvRendering::Resources::Texture* p_resource) override;
	};
}