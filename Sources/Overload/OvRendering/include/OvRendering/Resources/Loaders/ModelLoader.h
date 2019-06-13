/**
* @project: Overload
* @author: Overload Tech.
* @restrictions: This software may not be resold, redistributed or otherwise conveyed to a third party.
*/

#pragma once

#include <string>

#include "OvRendering/Resources/Model.h"
#include "OvRendering/Resources/Parsers/AssimpParser.h"

namespace OvRendering::Resources::Loaders
{
	/**
	* Handle the Model creation and destruction
	*/
	class API_OVRENDERING ModelLoader
	{
	public:
		/**
		* Disabled constructor
		*/
		ModelLoader() = delete;

		/**
		* Create a model
		* @param p_filepath
		* @param p_parserFlags
		*/
		static Model* Create(const std::string& p_filepath, Parsers::EModelParserFlags p_parserFlags = Parsers::EModelParserFlags::NONE);

		/**
		* Disabled constructor
		* @param p_modelInstance
		*/
		static bool Destroy(Model*& p_modelInstance);

	private:
		static Parsers::AssimpParser __ASSIMP;
	};
}