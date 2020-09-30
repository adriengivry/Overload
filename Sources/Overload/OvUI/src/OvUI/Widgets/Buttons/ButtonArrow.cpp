/**
* @project: Overload
* @author: Overload Tech.
* @licence: MIT
*/

#include "OvUI/Widgets/Buttons/ButtonArrow.h"

OvUI::Widgets::Buttons::ButtonArrow::ButtonArrow(ImGuiDir p_direction) :
	direction(p_direction)
{
}

void OvUI::Widgets::Buttons::ButtonArrow::_Draw_Impl()
{
	if (ImGui::ArrowButton(m_widgetID.c_str(), direction))
		ClickedEvent.Invoke();
}
