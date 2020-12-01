/**
* @project: Overload
* @author: Overload Tech.
* @licence: MIT
*/

#pragma once

#include <vector>

#include "OvRendering/Buffers/VertexArray.h"

namespace OvRendering::Resources
{
	/**
	* Interface for any mesh
	*/
	class IMesh
	{
	public:
		virtual void Bind() = 0;
		virtual void Unbind() = 0;
		virtual uint32_t GetVertexCount() = 0;
		virtual uint32_t GetIndexCount() = 0;
	};
}