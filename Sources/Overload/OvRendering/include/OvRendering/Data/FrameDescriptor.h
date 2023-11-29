/**
* @project: Overload
* @author: Overload Tech.
* @licence: MIT
*/

#pragma once

#include "OvRendering/Buffers/Framebuffer.h"
#include "OvRendering/Entities/Camera.h"

namespace OvRendering::Data
{
	/**
	* Describe how a given frame should be rendered
	*/
	struct FrameDescriptor
	{
		OvMaths::FVector3 clearColor = OvMaths::FVector3::Zero;
		bool clearColorBuffer = true;
		bool clearDepthBuffer = true;
		bool clearStencilBuffer = false;
		uint16_t renderWidth = 0;
		uint16_t renderHeight = 0;
		Buffers::Framebuffer* outputBuffer = nullptr;
	};
}
