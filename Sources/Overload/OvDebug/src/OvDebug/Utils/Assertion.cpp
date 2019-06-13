/**
* @project: Overload
* @author: Overload Tech.
* @restrictions: This software may not be resold, redistributed or otherwise conveyed to a third party.
*/

#include "OvDebug/Utils/Assertion.h"

#include <assert.h>

void OvDebug::Assertion::Assert(bool p_condition, const std::string& p_message)
{
	assert(p_condition && p_message.c_str());
}
