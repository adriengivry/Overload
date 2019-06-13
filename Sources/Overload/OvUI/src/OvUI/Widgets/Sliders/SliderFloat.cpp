/**
* @project: Overload
* @author: Overload Tech.
* @restrictions: This software may not be resold, redistributed or otherwise conveyed to a third party.
*/

#include "OvUI/Widgets/Sliders/SliderFloat.h"

OvUI::Widgets::Sliders::SliderFloat::SliderFloat
(
	float p_min,
	float p_max,
	float p_value,
	ESliderOrientation p_orientation,
	const std::string & p_label,
	const std::string & p_format
) : SliderSingleScalar<float>(ImGuiDataType_::ImGuiDataType_Float, p_min, p_max, p_value, p_orientation, p_label, p_format)
{
}