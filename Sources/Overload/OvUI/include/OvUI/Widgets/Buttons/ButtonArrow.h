/**
* @project: Overload
* @author: Overload Tech.
* @restrictions: This software may not be resold, redistributed or otherwise conveyed to a third party.
*/

#pragma once

#include <string>

#include "OvUI/Widgets/Buttons/AButton.h"

namespace OvUI::Widgets::Buttons
{
	/**
	* Button widget with an arrow image
	*/
	class API_OVUI ButtonArrow : public AButton
	{
	public:
		/**
		* Create the button
		* @param p_direction
		*/
		ButtonArrow(ImGuiDir p_direction = ImGuiDir_None);

	protected:
		void _Draw_Impl() override;

	public:
		ImGuiDir direction;
	};
}