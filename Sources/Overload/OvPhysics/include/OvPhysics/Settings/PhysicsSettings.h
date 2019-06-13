/**
* @project: Overload
* @author: Overload Tech.
* @restrictions: This software may not be resold, redistributed or otherwise conveyed to a third party.
*/

#pragma once

#include <OvMaths/FVector3.h>

namespace OvPhysics::Settings
{
	/**
	* Data structure to give to the PhysicsEngine constructor to setup its settings
	*/
	struct PhysicsSettings
	{
		OvMaths::FVector3 gravity = { 0.0f, -9.81f, 0.f };
	};
}