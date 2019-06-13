/**
* @project: Overload
* @author: Overload Tech.
* @restrictions: This software may not be resold, redistributed or otherwise conveyed to a third party.
*/

#pragma once

namespace OvUI::API
{
	/**
	* Interface for any drawable class
	*/
	class IDrawable
	{
	public:
		virtual void Draw() = 0;
	};
}
