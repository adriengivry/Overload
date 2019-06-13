/**
* @project: Overload
* @author: Overload Tech.
* @restrictions: This software may not be resold, redistributed or otherwise conveyed to a third party.
*/

#pragma once

#include <unordered_map>

#include "OvRendering/Resources/Mesh.h"

namespace OvRendering::Resources
{
	namespace Loaders { class ModelLoader; }

	/**
	* A model is a combinaison of meshes
	*/
	class API_OVRENDERING Model
	{
		friend class Loaders::ModelLoader;

	public:
		/**
		* Returns the meshes
		*/
		const std::vector<Mesh*>& GetMeshes() const;

		/**
		* Returns the material names
		*/
		const std::vector<std::string>& GetMaterialNames() const;

	private:
		Model(const std::string& p_path);
		~Model();

	public:
		const std::string path;

	private:
		std::vector<Mesh*> m_meshes;
		std::vector<std::string> m_materialNames;
	};
}