/**
* @project: Overload
* @author: Overload Tech.
* @restrictions: This software may not be resold, redistributed or otherwise conveyed to a third party.
*/

#pragma once

#include <stdint.h>

namespace OvUI::Internal
{
	/**
	* Simple union necessary for imgui textureID
	*/
	union TextureID
	{
		uint32_t d;
		void* raw;
	};
}