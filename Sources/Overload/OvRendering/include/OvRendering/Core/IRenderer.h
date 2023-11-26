/**
* @project: Overload
* @author: Overload Tech.
* @licence: MIT
*/

#pragma once

#include <optional>

#include "OvRendering/Data/RenderOutputDesc.h"
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
		virtual void BeginFrame(std::optional<Data::RenderOutputDesc> p_outputDesc) = 0;
		virtual void EndFrame() = 0;
		virtual Context::Driver& GetDriver() const = 0;
	};
}