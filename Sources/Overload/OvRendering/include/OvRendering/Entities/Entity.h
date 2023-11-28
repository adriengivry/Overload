/**
* @project: Overload
* @author: Overload Tech.
* @licence: MIT
*/

#pragma once

#include <memory>

#include <OvMaths/FTransform.h>
#include <OvTools/Utils/ReferenceOrValue.h>

namespace OvRendering::Entities
{
	/**
	* Represents an entity with a transformation in space
	*/
	class Entity
	{
	public:
		/**
		* Create the entity
		* @param p_transform
		*/
		Entity(OvMaths::FTransform* p_transform = nullptr);

		/**
		* Return the a reference to the current transform
		*/
		OvMaths::FTransform& GetTransform();

		/**
		* Return the a reference to the current transform
		*/
		const OvMaths::FTransform& GetTransform() const;

	private:
		OvTools::Utils::ReferenceOrValue<OvMaths::FTransform> m_transform;
	};
}
