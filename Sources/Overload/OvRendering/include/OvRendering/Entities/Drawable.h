/**
* @project: Overload
* @author: Overload Tech.
* @licence: MIT
*/

#pragma once

#include <OvMaths/FTransform.h>

#include "OvRendering/Resources/Mesh.h"
#include "OvRendering/Data/Material.h"

namespace OvRendering::Entities
{
	/**
	* Drawable entity
	*/
	struct Drawable
	{
		OvMaths::FMatrix4 modelMatrix;
		OvRendering::Resources::Mesh& mesh;
		OvRendering::Data::Material& material;
		OvRendering::Data::StateMask stateMask;
		OvMaths::FMatrix4 userMatrix; // Can be used to store anything you want :)
	};
}