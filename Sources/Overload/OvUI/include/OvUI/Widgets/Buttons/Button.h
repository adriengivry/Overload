/**
* @project: Overload
* @author: Overload Tech.
* @restrictions: This software may not be resold, redistributed or otherwise conveyed to a third party.
*/

#pragma once

#include <string>

#include <OvMaths/FVector2.h>

#include "OvUI/Widgets/Buttons/AButton.h"
#include "OvUI/Types/Color.h"

namespace OvUI::Widgets::Buttons
{
	/**
	* Simple button widget
	*/
	class API_OVUI Button : public AButton
	{
	public:
		/**
		* Constructor
		* @param p_label
		* @param p_size
		* @param p_disabled
		*/
		Button(const std::string& p_label = "", const OvMaths::FVector2& p_size = OvMaths::FVector2(0.f, 0.f), bool p_disabled = false);

	protected:
		void _Draw_Impl() override;

	public:
		std::string label;
		OvMaths::FVector2 size;
		bool disabled = false;

		Types::Color idleBackgroundColor;
		Types::Color hoveredBackgroundColor;
		Types::Color clickedBackgroundColor;

		Types::Color textColor;
	};
}