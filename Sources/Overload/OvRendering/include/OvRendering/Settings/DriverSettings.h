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
    * Settings that are sent to the driver at construction
    */
    struct API_OVRENDERING DriverSettings
    {
        bool debugMode = false;
    };
}