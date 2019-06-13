/**
* @project: Overload
* @author: Overload Tech.
* @restrictions: This software may not be resold, redistributed or otherwise conveyed to a third party.
*/

#pragma once

#include "OvUI/Panels/APanelTransformable.h"

namespace OvUI::Panels
{
	/**
	* A simple panel that is transformable and without decorations (No background)
	*/
	class API_OVUI PanelUndecorated : public APanelTransformable
	{
	public:
		void _Draw_Impl() override;

	private:
		int CollectFlags();
	};
}