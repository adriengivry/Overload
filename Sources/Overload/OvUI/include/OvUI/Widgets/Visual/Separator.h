/**
* @project: Overload
* @author: Overload Tech.
* @restrictions: This software may not be resold, redistributed or otherwise conveyed to a third party.
*/

#pragma once

#include "OvUI/Widgets/AWidget.h"

namespace OvUI::Widgets::Visual
{
	/**
	* Simple widget that display a separator
	*/
	class API_OVUI Separator : public AWidget
	{
	protected:
		void _Draw_Impl() override;
	};
}