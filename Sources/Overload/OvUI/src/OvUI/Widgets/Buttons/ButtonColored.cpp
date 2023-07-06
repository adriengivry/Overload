/**
* @project: Overload
* @author: Overload Tech.
* @licence: MIT
*/

#include "OvUI/Widgets/Buttons/ButtonColored.h"

OvUI::Widgets::Buttons::ButtonColored::ButtonColored(const std::string & p_label, const Types::Color& p_color, const OvMaths::FVector2& p_size, bool p_enableAlpha) :
	label(p_label), color(p_color), size(p_size), enableAlpha(p_enableAlpha)
{
}

void OvUI::Widgets::Buttons::ButtonColored::_Draw_Impl()
{
	auto colorButtonVector4 = color.ToVector4();

	if (ImGui::ColorButton((label + m_widgetID).c_str(), colorButtonVector4, !enableAlpha ? ImGuiColorEditFlags_NoAlpha : 0, size))
		ClickedEvent.Invoke();

	color = colorButtonVector4;
}
