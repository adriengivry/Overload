/**
* @project: Overload
* @author: Overload Tech.
* @licence: MIT
*/

#include "OvCore/ResourceManagement/ModelManager.h"

OvRendering::Resources::Model* OvCore::ResourceManagement::ModelManager::CreateResource(const std::string& p_path)
{
	OvRendering::Resources::Parsers::EModelParserFlags flags = OvRendering::Resources::Parsers::EModelParserFlags::NONE;

	flags |= OvRendering::Resources::Parsers::EModelParserFlags::TRIANGULATE;
	flags |= OvRendering::Resources::Parsers::EModelParserFlags::GEN_SMOOTH_NORMALS;
	flags |= OvRendering::Resources::Parsers::EModelParserFlags::OPTIMIZE_MESHES;
	flags |= OvRendering::Resources::Parsers::EModelParserFlags::OPTIMIZE_GRAPH;
	flags |= OvRendering::Resources::Parsers::EModelParserFlags::FIND_INSTANCES;
	flags |= OvRendering::Resources::Parsers::EModelParserFlags::CALC_TANGENT_SPACE;
	flags |= OvRendering::Resources::Parsers::EModelParserFlags::JOIN_IDENTICAL_VERTICES;
	flags |= OvRendering::Resources::Parsers::EModelParserFlags::DEBONE;
	flags |= OvRendering::Resources::Parsers::EModelParserFlags::FIND_INVALID_DATA;
	flags |= OvRendering::Resources::Parsers::EModelParserFlags::IMPROVE_CACHE_LOCALITY;
	flags |= OvRendering::Resources::Parsers::EModelParserFlags::GEN_UV_COORDS;
	flags |= OvRendering::Resources::Parsers::EModelParserFlags::PRE_TRANSFORM_VERTICES;

	std::string realPath = GetRealPath(p_path);
	auto model = OvRendering::Resources::Loaders::ModelLoader::Create(realPath, flags);
	if (model)
		*reinterpret_cast<std::string*>(reinterpret_cast<char*>(model) + offsetof(OvRendering::Resources::Model, path)) = p_path; // Force the resource path to fit the given path

	return model;
}

void OvCore::ResourceManagement::ModelManager::DestroyResource(OvRendering::Resources::Model* p_resource)
{
	OvRendering::Resources::Loaders::ModelLoader::Destroy(p_resource);
}
