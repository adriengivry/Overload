/**
* @project: Overload
* @author: Overload Tech.
* @restrictions: This software may not be resold, redistributed or otherwise conveyed to a third party.
*/

#include "OvRendering/Resources/Mesh.h"

OvRendering::Resources::Mesh::Mesh(const std::vector<Geometry::Vertex>& p_vertices, const std::vector<uint32_t>& p_indices, uint32_t p_materialIndex) :
	m_vertexCount(static_cast<uint32_t>(p_vertices.size())),
	m_indicesCount(static_cast<uint32_t>(p_indices.size())),
	m_materialIndex(p_materialIndex)
{
	CreateBuffers(p_vertices, p_indices);
}

void OvRendering::Resources::Mesh::Bind()
{
	m_vertexArray.Bind();
}

void OvRendering::Resources::Mesh::Unbind()
{
	m_vertexArray.Unbind();
}

uint32_t OvRendering::Resources::Mesh::GetVertexCount()
{
	return m_vertexCount;
}

uint32_t OvRendering::Resources::Mesh::GetIndexCount()
{
	return m_indicesCount;
}

uint32_t OvRendering::Resources::Mesh::GetMaterialIndex() const
{
	return m_materialIndex;
}

void OvRendering::Resources::Mesh::CreateBuffers(const std::vector<Geometry::Vertex>& p_vertices, const std::vector<uint32_t>& p_indices)
{
	std::vector<float> vertexData;

	std::vector<unsigned int> rawIndices;

	for (const auto& vertex : p_vertices)
	{
		vertexData.push_back(vertex.position[0]);
		vertexData.push_back(vertex.position[1]);
		vertexData.push_back(vertex.position[2]);

		vertexData.push_back(vertex.texCoords[0]);
		vertexData.push_back(vertex.texCoords[1]);

		vertexData.push_back(vertex.normals[0]);
		vertexData.push_back(vertex.normals[1]);
		vertexData.push_back(vertex.normals[2]);

		vertexData.push_back(vertex.tangent[0]);
		vertexData.push_back(vertex.tangent[1]);
		vertexData.push_back(vertex.tangent[2]);

		vertexData.push_back(vertex.bitangent[0]);
		vertexData.push_back(vertex.bitangent[1]);
		vertexData.push_back(vertex.bitangent[2]);
	}

	m_vertexBuffer	= std::make_unique<Buffers::VertexBuffer<float>>(vertexData);
	m_indexBuffer	= std::make_unique<Buffers::IndexBuffer>(const_cast<uint32_t*>(p_indices.data()), p_indices.size());

	uint64_t vertexSize = sizeof(Geometry::Vertex);

	m_vertexArray.BindAttribute(0, *m_vertexBuffer, Buffers::EType::FLOAT, 3, vertexSize, 0);
	m_vertexArray.BindAttribute(1, *m_vertexBuffer,	Buffers::EType::FLOAT, 2, vertexSize, sizeof(float) * 3);
	m_vertexArray.BindAttribute(2, *m_vertexBuffer,	Buffers::EType::FLOAT, 3, vertexSize, sizeof(float) * 5);
	m_vertexArray.BindAttribute(3, *m_vertexBuffer,	Buffers::EType::FLOAT, 3, vertexSize, sizeof(float) * 8);
	m_vertexArray.BindAttribute(4, *m_vertexBuffer,	Buffers::EType::FLOAT, 3, vertexSize, sizeof(float) * 11);
}
