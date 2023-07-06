/**
* @project: Overload
* @author: Overload Tech.
* @licence: MIT
*/

#include "OvUI/Widgets/Layout/Dummy.h"

OvUI::Widgets::Layout::Dummy::Dummy(const OvMaths::FVector2& p_size) : size(p_size)
{
}

void OvUI::Widgets::Layout::Dummy::_Draw_Impl()
{
	ImGui::Dummy(size);
}
