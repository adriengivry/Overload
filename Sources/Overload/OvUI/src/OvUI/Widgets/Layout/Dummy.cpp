/**
* @project: Overload
* @author: Overload Tech.
* @restrictions: This software may not be resold, redistributed or otherwise conveyed to a third party.
*/

#include "OvUI/Widgets/Layout/Dummy.h"
#include "OvUI/Internal/Converter.h"

OvUI::Widgets::Layout::Dummy::Dummy(const OvMaths::FVector2& p_size) : size(p_size)
{
}

void OvUI::Widgets::Layout::Dummy::_Draw_Impl()
{
	ImGui::Dummy(Internal::Converter::ToImVec2(size));
}
