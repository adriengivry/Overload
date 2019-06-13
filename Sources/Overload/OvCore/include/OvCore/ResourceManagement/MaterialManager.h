/**
* @project: Overload
* @author: Overload Tech.
* @restrictions: This software may not be resold, redistributed or otherwise conveyed to a third party.
*/

#pragma once

#include "OvCore/Resources/Loaders/MaterialLoader.h"
#include "OvCore/ResourceManagement/AResourceManager.h"

namespace OvCore::ResourceManagement
{
	/**
	* ResourceManager of materials
	*/
	class API_OVCORE MaterialManager : public AResourceManager<OvCore::Resources::Material>
	{
	public:
		/**
		* Create the resource identified by the given path
		* @param p_path
		*/
		virtual OvCore::Resources::Material* CreateResource(const std::string & p_path) override;

		/**
		* Destroy the given resource
		* @param p_resource
		*/
		virtual void DestroyResource(OvCore::Resources::Material* p_resource) override;
	};
}