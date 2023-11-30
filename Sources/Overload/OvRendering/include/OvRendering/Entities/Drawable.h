/**
* @project: Overload
* @author: Overload Tech.
* @licence: MIT
*/

#pragma once

#include <OvMaths/FTransform.h> // TODO: Unused right now, might want to use it instead of model matrix

#include "OvRendering/Resources/Mesh.h"
#include "OvRendering/Data/Material.h"

#include <set>

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
		std::set<uint8_t> tags;

		bool HasTag(uint8_t p_tag)
		{
			return tags.find(p_tag) != tags.end();
		}
	};
}