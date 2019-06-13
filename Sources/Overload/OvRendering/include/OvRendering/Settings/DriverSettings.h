/**
* @project: Overload
* @author: Overload Tech.
* @restrictions: This software may not be resold, redistributed or otherwise conveyed to a third party.
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