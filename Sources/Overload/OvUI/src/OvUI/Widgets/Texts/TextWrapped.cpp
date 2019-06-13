/**
* @project: Overload
* @author: Overload Tech.
* @restrictions: This software may not be resold, redistributed or otherwise conveyed to a third party.
*/

#include "OvUI/Widgets/Texts/TextWrapped.h"

OvUI::Widgets::Texts::TextWrapped::TextWrapped(const std::string & p_content) :
	Text(p_content)
{
}

void OvUI::Widgets::Texts::TextWrapped::_Draw_Impl()
{
	ImGui::TextWrapped(content.c_str());
}
