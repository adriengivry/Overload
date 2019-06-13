/**
* @project: Overload
* @author: Overload Tech.
* @restrictions: This software may not be resold, redistributed or otherwise conveyed to a third party.
*/

#pragma once

#include <OvMaths/FVector2.h>

#include "OvUI/Widgets/AWidget.h"

namespace OvUI::Widgets::Layout
{
	/**
	* Dummy widget that takes the given size as space in the panel
	*/
	class API_OVUI Dummy : public AWidget
	{
	public:
		/**
		* Constructor
		* @param p_size
		*/
		Dummy(const OvMaths::FVector2& p_size = { 0.0f, 0.0f });

	protected:
		void _Draw_Impl() override;

	public:
		OvMaths::FVector2 size;
	};
}