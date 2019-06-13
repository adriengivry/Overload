/**
* @project: Overload
* @author: Overload Tech.
* @restrictions: This software may not be resold, redistributed or otherwise conveyed to a third party.
*/

#pragma once

#include <bullet/btBulletCollisionCommon.h>

#include <vector>

#include "OvPhysics/API/Export.h"
#include "OvPhysics/Entities/PhysicalObject.h"

namespace OvPhysics::Entities
{
	/**
	* Data structure that holds raycast hit information
	*/
	struct API_OVPHYSICS RaycastHit
	{
		Entities::PhysicalObject* FirstResultObject = nullptr;
		std::vector<Entities::PhysicalObject*> ResultObjects;
	};
}