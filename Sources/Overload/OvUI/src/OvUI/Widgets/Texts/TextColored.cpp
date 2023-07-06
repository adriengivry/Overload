/**
* @project: Overload
* @author: Overload Tech.
* @licence: MIT
*/

#include "OvUI/Widgets/Texts/TextColored.h"

OvUI::Widgets::Texts::TextColored::TextColored(const std::string& p_content, const Types::Color& p_color) :
	Text(p_content), color(p_color)
{
}

void OvUI::Widgets::Texts::TextColored::_Draw_Impl()
{
	ImGui::TextColored(color.ToVector4(), content.c_str());
}
