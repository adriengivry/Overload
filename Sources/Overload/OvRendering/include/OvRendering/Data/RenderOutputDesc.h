/**
* @project: Overload
* @author: Overload Tech.
* @licence: MIT
*/

#pragma once

#include "OvRendering/Buffers/Framebuffer.h"

namespace OvRendering::Data
{
	/**
	* Descriptor for a render output
	*/
	struct RenderOutputDesc
	{
		const OvRendering::Buffers::Framebuffer& framebuffer;
	};
}
