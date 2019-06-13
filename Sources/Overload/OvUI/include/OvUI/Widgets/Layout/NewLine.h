/**
* @project: Overload
* @author: Overload Tech.
* @restrictions: This software may not be resold, redistributed or otherwise conveyed to a third party.
*/

#pragma once

#include "OvUI/Widgets/AWidget.h"

namespace OvUI::Widgets::Layout
{
	/**
	* Widget that adds an empty line to the panel
	*/
	class API_OVUI NewLine : public AWidget
	{
	protected:
		void _Draw_Impl() override;
	};
}