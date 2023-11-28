/**
* @project: Overload
* @author: Overload Tech.
* @licence: MIT
*/

#include "OvRendering/Entities/Entity.h"

OvRendering::Entities::Entity::Entity(OvMaths::FTransform* p_transform) : m_transform(p_transform)
{
}

OvMaths::FTransform& OvRendering::Entities::Entity::GetTransform()
{
    return m_transform.GetRef();
}

const OvMaths::FTransform& OvRendering::Entities::Entity::GetTransform() const
{
    return m_transform.Get();
}
