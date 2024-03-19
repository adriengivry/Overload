/**
* @project: Overload
* @author: Overload Tech.
* @licence: MIT
*/

#pragma once

#include <OvMaths/FTransform.h> // TODO: Unused right now, might want to use it instead of model matrix
#include <OvTools/Utils/OptRef.h>

#include "OvRendering/Resources/Mesh.h"
#include "OvRendering/Data/Material.h"
#include "OvRendering/Data/Describable.h"

namespace OvRendering::Entities
{
	/**
	* Drawable entity
	*/
	struct Drawable : public Data::Describable
	{
		OvTools::Utils::OptRef<OvRendering::Resources::Mesh> mesh;
		OvTools::Utils::OptRef<OvRendering::Data::Material> material;
		OvRendering::Data::StateMask stateMask;
		OvRendering::Settings::EPrimitiveMode primitiveMode = OvRendering::Settings::EPrimitiveMode::TRIANGLES;
	};
}