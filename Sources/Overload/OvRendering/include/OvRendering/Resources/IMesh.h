/**
* @project: Overload
* @author: Overload Tech.
* @restrictions: This software may not be resold, redistributed or otherwise conveyed to a third party.
*/

#pragma once

#include <vector>

#include "OvRendering/Buffers/VertexArray.h"

namespace OvRendering::Resources
{
	/**
	* Interface for any mesh
	*/
	class API_OVRENDERING IMesh
	{
	public:
		virtual void Bind() = 0;
		virtual void Unbind() = 0;
		virtual uint32_t GetVertexCount() = 0;
		virtual uint32_t GetIndexCount() = 0;
	};
}