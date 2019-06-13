/**
* @project: Overload
* @author: Overload Tech.
* @restrictions: This software may not be resold, redistributed or otherwise conveyed to a third party.
*/

#pragma once

#include <sol.hpp>

namespace OvCore::Scripting
{
	/**
	* Handle lua maths binding
	*/
	class LuaMathsBinder
	{
	public:
		/**
		* Bind engine maths-related features to lua
		*/
		static void BindMaths(sol::state& p_luaState);
	};
}