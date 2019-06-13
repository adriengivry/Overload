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
	* Simple widget that display a bullet point
	*/
	class API_OVUI Bullet : public AWidget
	{
	protected:
		virtual void _Draw_Impl() override;
	};
}