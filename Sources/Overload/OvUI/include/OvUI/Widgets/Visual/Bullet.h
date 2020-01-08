/**
* @project: Overload
* @author: Overload Tech.
* @licence: MIT
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