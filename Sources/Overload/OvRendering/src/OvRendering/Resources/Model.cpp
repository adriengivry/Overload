/**
* @project: Overload
* @author: Overload Tech.
* @restrictions: This software may not be resold, redistributed or otherwise conveyed to a third party.
*/

#include <algorithm>

#include "OvRendering/Resources/Model.h"

const OvRendering::Geometry::BoundingSphere& OvRendering::Resources::Model::GetBoundingSphere() const
{
	return m_boundingSphere;
}

OvRendering::Resources::Model::Model(const std::string & p_path) : path(p_path)
{
}

OvRendering::Resources::Model::~Model()
{
	for (auto mesh : m_meshes)
		delete mesh;
}

void OvRendering::Resources::Model::ComputeBoundingSphere()
{
	for (auto mesh : m_meshes)
	{
		m_boundingSphere.radius = std::max(m_boundingSphere.radius, mesh->GetBoundingSphere().radius);
	}
}

const std::vector<OvRendering::Resources::Mesh*>& OvRendering::Resources::Model::GetMeshes() const
{
	return m_meshes;
}

const std::vector<std::string>& OvRendering::Resources::Model::GetMaterialNames() const
{
	return m_materialNames;
}
