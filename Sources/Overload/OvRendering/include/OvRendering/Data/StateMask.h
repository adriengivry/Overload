/**
* @project: Overload
* @author: Overload Tech.
* @licence: MIT
*/

#pragma once

#include <cstdint>

namespace OvRendering::Data
{
	/**
	* Represents the current driver state and allow for efficient context switches
	*/
	struct StateMask
	{
		union
		{
			struct
			{
				uint8_t depthWriting : 1;
				uint8_t colorWriting : 1;
				uint8_t blendable : 1;
				uint8_t depthTest : 1;
				uint8_t backfaceCulling : 1;
				uint8_t frontfaceCulling : 1;
			};

			uint8_t mask;
		};
	};
}
