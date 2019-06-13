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
	* Simple widget to display a long text with word-wrap on a panel
	*/
	class API_OVUI TextWrapped : public Text
	{
	public:
		/**
		* Constructor
		* @param p_content
		*/
		TextWrapped(const std::string& p_content = "");

	protected:
		virtual void _Draw_Impl() override;
	};
}