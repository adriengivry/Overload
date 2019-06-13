/**
* @project: Overload
* @author: Overload Tech.
* @restrictions: This software may not be resold, redistributed or otherwise conveyed to a third party.
*/

#include "OvCore/Scripting/LuaBinder.h"

#include "OvCore/Scripting/LuaMathsBinder.h"
#include "OvCore/Scripting/LuaActorBinder.h"
#include "OvCore/Scripting/LuaComponentBinder.h"
#include "OvCore/Scripting/LuaGlobalsBinder.h"

void OvCore::Scripting::LuaBinder::CallBinders(sol::state& p_luaState)
{
	auto& L = p_luaState;

	LuaMathsBinder::BindMaths(L);
	LuaActorBinder::BindActor(L);
	LuaComponentBinder::BindComponent(L);
	LuaGlobalsBinder::BindGlobals(L);
}