/**
* @project: Overload
* @author: Overload Tech.
* @licence: MIT
*/

#include "OvUI/Widgets/Buttons/ButtonSmall.h"

OvUI::Widgets::Buttons::ButtonSmall::ButtonSmall(const std::string& p_label) :
	label(p_label)
{
	auto& style = ImGui::GetStyle();

	idleBackgroundColor		= style.Colors[ImGuiCol_Button];
	hoveredBackgroundColor	= style.Colors[ImGuiCol_ButtonHovered];
	clickedBackgroundColor	= style.Colors[ImGuiCol_ButtonActive];
	textColor				= style.Colors[ImGuiCol_Text];
}

void OvUI::Widgets::Buttons::ButtonSmall::_Draw_Impl()
{
	auto& style = ImGui::GetStyle();

	auto defaultIdleColor		= style.Colors[ImGuiCol_Button];
	auto defaultHoveredColor	= style.Colors[ImGuiCol_ButtonHovered];
	auto defaultClickedColor	= style.Colors[ImGuiCol_ButtonActive];
	auto defaultTextColor		= style.Colors[ImGuiCol_Text];

	style.Colors[ImGuiCol_Button]			= idleBackgroundColor.ToVector4();
	style.Colors[ImGuiCol_ButtonHovered]	= hoveredBackgroundColor.ToVector4();
	style.Colors[ImGuiCol_ButtonActive]		= clickedBackgroundColor.ToVector4();
	style.Colors[ImGuiCol_Text]				= textColor.ToVector4();

	if (ImGui::SmallButton((label + m_widgetID).c_str()))
		ClickedEvent.Invoke();

	style.Colors[ImGuiCol_Button]			= defaultIdleColor;
	style.Colors[ImGuiCol_ButtonHovered]	= defaultHoveredColor;
	style.Colors[ImGuiCol_ButtonActive]		= defaultClickedColor;
	style.Colors[ImGuiCol_Text]				= defaultTextColor;
}
