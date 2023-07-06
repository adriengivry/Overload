/**
* @project: Overload
* @author: Overload Tech.
* @licence: MIT
*/

#include "OvUI/Widgets/Buttons/ButtonImage.h"

OvUI::Widgets::Buttons::ButtonImage::ButtonImage(uint32_t p_textureID, const OvMaths::FVector2 & p_size) :
	textureID{ p_textureID }, size(p_size)
{
}

void OvUI::Widgets::Buttons::ButtonImage::_Draw_Impl()
{
	if (disabled)
		ImGui::BeginDisabled();

	if (ImGui::ImageButton(textureID.raw, size, OvMaths::FVector2(0.f, 1.f), OvMaths::FVector2(1.f, 0.f), -1, background.ToVector4(), tint.ToVector4()))
		ClickedEvent.Invoke();

	if (disabled)
		ImGui::EndDisabled();
}
