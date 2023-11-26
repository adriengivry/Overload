/**
* @project: Overload
* @author: Overload Tech.
* @licence: MIT
*/

#pragma once

#include <functional>

namespace OvRendering::Data
{
	/**
	* Holds a set of renderer callbacks
	*/
	struct RendererCallbacks
	{
		std::function<void(OvMaths::FMatrix4)> modelMatrixSender;
		std::function<void(OvMaths::FMatrix4)> userMatrixSender;
	};
}
