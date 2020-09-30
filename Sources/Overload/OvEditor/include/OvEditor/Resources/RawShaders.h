/**
* @project: Overload
* @author: Overload Tech.
* @licence: MIT
*/

#pragma once

#include <string>

namespace OvEditor::Resources
{
	/**
	* Defines some editor shaders
	*/
	class RawShaders
	{
	public:
		/**
		* Returns the grid shader
		*/
		static std::pair<std::string, std::string> GetGrid();

		/**
		* Returns the gizmo shader
		*/
		static std::pair<std::string, std::string> GetGizmo();

		/**
		* Returns the billboard shader
		*/
		static std::pair<std::string, std::string> GetBillboard();
	};
}