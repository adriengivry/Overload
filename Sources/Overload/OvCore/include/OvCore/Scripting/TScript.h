/**
* @project: Overload
* @author: Overload Tech.
* @licence: MIT
*/

#pragma once

#include <OvCore/Scripting/EScriptingLanguage.h>

namespace OvCore::Scripting
{
	/**
	* Interface for any scripting backend to implement.
	* This class defines the necessary methods that a scripting engine should provide
	* to interact with the engine's components and handle various lifecycle events.
	*/
	template<EScriptingLanguage Language, class ScriptContext>
	class TScript
	{
	public:
		TScript() {}
		virtual ~TScript() {}

		/**
		* Checks if the script context is valid.
		* @return True if the context is valid, false otherwise.
		*/
		bool IsValid() const;

	public:
		ScriptContext m_context;
	};
}
