/**
* @project: Overload
* @author: Overload Tech.
* @licence: MIT
*/

#pragma once

#include <vector>
#include <memory>

#include <OvCore/Scripting/EScriptingLanguage.h>
#include <OvCore/Scripting/TScriptEngine.h>
#include <OvCore/Scripting/TScript.h>

namespace OvCore::Scripting
{
	struct NullScriptEngineContext {};
	struct NullScriptContext {};

	using NullScriptEngine = TScriptEngine<EScriptingLanguage::NONE, NullScriptEngineContext>;
	using NullScript = TScript<EScriptingLanguage::NONE, NullScriptContext>;
}
