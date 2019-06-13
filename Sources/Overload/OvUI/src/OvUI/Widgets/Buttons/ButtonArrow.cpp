/**
* @project: Overload
* @author: Overload Tech.
* @restrictions: This software may not be resold, redistributed or otherwise conveyed to a third party.
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
