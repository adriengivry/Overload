/**
* @project: Overload
* @author: Overload Tech.
* @licence: MIT
*/

#include "OvRendering/Features/DebugShapeRenderFeature.h"
#include "OvRendering/Core/CompositeRenderer.h"
#include "OvRendering/Resources/Loaders/ShaderLoader.h"
#include "OvRendering/Utils/Conversions.h"

OvRendering::Features::DebugShapeRenderFeature::DebugShapeRenderFeature(Core::CompositeRenderer& p_renderer)
	: ARenderFeature(p_renderer)
{
	std::vector<Geometry::Vertex> vertices;
	vertices.push_back
	({
		0, 0, 0,
		0, 0,
		0, 0, 0,
		0, 0, 0,
		0, 0, 0
	});
	vertices.push_back
	({
		0, 0, 0,
		0, 0,
		0, 0, 0,
		0, 0, 0,
		0, 0, 0
	});

	m_lineMesh = new Resources::Mesh(vertices, { 0, 1 }, 0);

	// TODO: Move these out of here, maybe we could have proper source files for these.
	std::string vertexShader = R"(
#version 430 core

uniform vec3 start;
uniform vec3 end;
uniform mat4 viewProjection;

void main()
{
	vec3 position = gl_VertexID == 0 ? start : end;
	gl_Position = viewProjection * vec4(position, 1.0);
}

)";

	std::string fragmentShader = R"(
#version 430 core

uniform vec3 color;

out vec4 FRAGMENT_COLOR;

void main()
{
	FRAGMENT_COLOR = vec4(color, 1.0);
}
)";

	m_lineShader = OvRendering::Resources::Loaders::ShaderLoader::CreateFromSource(vertexShader, fragmentShader);
}

OvRendering::Features::DebugShapeRenderFeature::~DebugShapeRenderFeature()
{
	delete m_lineMesh;
	OvRendering::Resources::Loaders::ShaderLoader::Destroy(m_lineShader);
}

void OvRendering::Features::DebugShapeRenderFeature::OnBeginFrame(const Data::FrameDescriptor& p_frameDescriptor)
{
	SetViewProjection(
		p_frameDescriptor.camera->GetProjectionMatrix() *
		p_frameDescriptor.camera->GetViewMatrix()
	);
}

void OvRendering::Features::DebugShapeRenderFeature::SetViewProjection(const OvMaths::FMatrix4& p_viewProjection)
{
	m_lineShader->Bind();
	m_lineShader->SetUniformMat4("viewProjection", p_viewProjection);
	m_lineShader->Unbind();
}

void OvRendering::Features::DebugShapeRenderFeature::DrawLine(
	OvRendering::Data::PipelineState p_pso,
	const OvMaths::FVector3& p_start,
	const OvMaths::FVector3& p_end,
	const OvMaths::FVector3& p_color,
	float p_lineWidth
)
{
	m_lineShader->Bind();

	m_lineShader->SetUniformVec3("start", p_start);
	m_lineShader->SetUniformVec3("end", p_end);
	m_lineShader->SetUniformVec3("color", p_color);

	p_pso.rasterizationMode = Settings::ERasterizationMode::LINE;
	p_pso.lineWidthPow2 = Utils::Conversions::FloatToPow2(p_lineWidth);
	m_renderer.DrawMesh(p_pso, *m_lineMesh, Settings::EPrimitiveMode::LINES);

	m_lineShader->Unbind();
}
