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
                uint8_t culling : 1;
                uint8_t depthTest : 1;
                uint8_t backfaceCulling : 1;
                uint8_t frontfaceCulling : 1;
            };

            uint8_t mask;
        };

        StateMask() : mask(0) {}

        StateMask(uint8_t mask) : mask(mask) {}

        StateMask(bool p_depthWriting, bool p_colorWriting, bool p_blendable, bool p_backfaceCulling, bool p_frontfaceCulling, bool p_depthTest)
        {
            depthWriting = p_depthWriting;
            colorWriting = p_colorWriting;
            blendable = p_blendable;
            culling = (p_backfaceCulling || p_frontfaceCulling);
            depthTest = p_depthTest;
            backfaceCulling = p_backfaceCulling;
            frontfaceCulling = p_frontfaceCulling;
        }
    };
}
