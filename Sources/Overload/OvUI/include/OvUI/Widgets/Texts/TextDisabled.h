/**
* @project: Overload
* @author: Overload Tech.
* @restrictions: This software may not be resold, redistributed or otherwise conveyed to a third party.
*/

#pragma once

#include "OvUI/Widgets/Texts/Text.h"

namespace OvUI::Widgets::Texts
{
	/**
	* Widget to display a disabled text on a panel
	*/
	class API_OVUI TextDisabled : public Text
	{
	public:
		/**
		* Constructor
		* @param p_content
		*/
		TextDisabled(const std::string& p_content = "");

	protected:
		virtual void _Draw_Impl() override;
	};
}