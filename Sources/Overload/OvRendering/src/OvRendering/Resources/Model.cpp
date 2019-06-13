/**
* @project: Overload
* @author: Overload Tech.
* @restrictions: This software may not be resold, redistributed or otherwise conveyed to a third party.
*/

#include "OvRendering/Resources/Model.h"

OvRendering::Resources::Model::Model(const std::string & p_path) : path(p_path)
{
}

OvRendering::Resources::Model::~Model()
{
	for (auto mesh : m_meshes)
		delete mesh;
}

const std::vector<OvRendering::Resources::Mesh*>& OvRendering::Resources::Model::GetMeshes() const
{
	return m_meshes;
}

const std::vector<std::string>& OvRendering::Resources::Model::GetMaterialNames() const
{
	return m_materialNames;
}
