/**
* @project: Overload
* @author: Overload Tech.
* @licence: MIT
*/

#pragma once

#include "OvRendering/API/Export.h"

namespace OvRendering::Settings
{
    /**
    * Projection modes, mostly used for cameras
    */
    enum class EProjectionMode
    {
        ORTHOGRAPHIC,
        PERSPECTIVE
    };
}