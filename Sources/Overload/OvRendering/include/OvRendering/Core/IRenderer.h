/**
* @project: Overload
* @author: Overload Tech.
* @licence: MIT
*/

#pragma once

#include <optional>

#include "OvRendering/Data/FrameDescriptor.h"
#include "OvRendering/Context/Driver.h"

namespace OvRendering::Data
{
	struct FrameInfo;
}

namespace OvRendering::Core
{
	class IRenderer
	{
	public:
		virtual void BeginFrame(const Data::FrameDescriptor& p_frameDescriptor) = 0;
		virtual void EndFrame() = 0;
		virtual Context::Driver& GetDriver() const = 0;
	};
}