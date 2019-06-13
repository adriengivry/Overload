/**
* @project: Overload
* @author: Overload Tech.
* @restrictions: This software may not be resold, redistributed or otherwise conveyed to a third party.
*/

#pragma once

#include <vector>

#include "OvUI/Internal/WidgetContainer.h"

namespace OvUI::Widgets::Layout
{
	/**
	* Widget that can contains other widgets
	*/
	class API_OVUI Group : public AWidget, public Internal::WidgetContainer
	{
	protected:
		virtual void _Draw_Impl() override;
	};
}