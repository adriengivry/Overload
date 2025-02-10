/**
* @project: Overload
* @author: Overload Tech.
* @licence: MIT
*/

#pragma once

namespace OvCore::Scripting
{
	/**
	* Interface for scripting backends to implement and store script contexts
	*/
	class IScriptContext
	{
	public:
		/**
		* Returns true if the context is valid
		*/
		virtual bool IsValid() const = 0;
	};
}
