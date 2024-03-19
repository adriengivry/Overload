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
	m_lineMaterial = std::make_unique<OvRendering::Data::Material>(m_lineShader);
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
	m_lineMaterial->Set("start", p_start);
	m_lineMaterial->Set("end", p_end);
	m_lineMaterial->Set("color", p_color);

	p_pso.rasterizationMode = Settings::ERasterizationMode::LINE;
	p_pso.lineWidthPow2 = Utils::Conversions::FloatToPow2(p_lineWidth);

	OvRendering::Entities::Drawable drawable;
	drawable.material = *m_lineMaterial;
	drawable.mesh = m_lineMesh;
	drawable.stateMask = m_lineMaterial->GenerateStateMask();
	drawable.primitiveMode = Settings::EPrimitiveMode::LINES;

	m_renderer.DrawEntity(p_pso, drawable);

	m_lineShader->Unbind();
}

void OvRendering::Features::DebugShapeRenderFeature::DrawBox(
	OvRendering::Data::PipelineState p_pso,
	const OvMaths::FVector3& p_position,
	const OvMaths::FQuaternion& p_rotation,
	const OvMaths::FVector3& p_size,
	const OvMaths::FVector3& p_color,
	float p_lineWidth
)
{
	DrawLine(p_pso, p_position + p_rotation * OvMaths::FVector3{ -p_size.x, -p_size.y, -p_size.z }, p_position + p_rotation * OvMaths::FVector3{ -p_size.x, -p_size.y, +p_size.z }, p_color, p_lineWidth);
	DrawLine(p_pso, p_position + p_rotation * OvMaths::FVector3{ -p_size.x, p_size.y, -p_size.z }, p_position + p_rotation * OvMaths::FVector3{ -p_size.x, +p_size.y, +p_size.z }, p_color, p_lineWidth);
	DrawLine(p_pso, p_position + p_rotation * OvMaths::FVector3{ -p_size.x, -p_size.y, -p_size.z }, p_position + p_rotation * OvMaths::FVector3{ -p_size.x, +p_size.y, -p_size.z }, p_color, p_lineWidth);
	DrawLine(p_pso, p_position + p_rotation * OvMaths::FVector3{ -p_size.x, -p_size.y, +p_size.z }, p_position + p_rotation * OvMaths::FVector3{ -p_size.x, +p_size.y, +p_size.z }, p_color, p_lineWidth);
	DrawLine(p_pso, p_position + p_rotation * OvMaths::FVector3{ +p_size.x, -p_size.y, -p_size.z }, p_position + p_rotation * OvMaths::FVector3{ +p_size.x, -p_size.y, +p_size.z }, p_color, p_lineWidth);
	DrawLine(p_pso, p_position + p_rotation * OvMaths::FVector3{ +p_size.x, p_size.y, -p_size.z }, p_position + p_rotation * OvMaths::FVector3{ +p_size.x, +p_size.y, +p_size.z }, p_color, p_lineWidth);
	DrawLine(p_pso, p_position + p_rotation * OvMaths::FVector3{ +p_size.x, -p_size.y, -p_size.z }, p_position + p_rotation * OvMaths::FVector3{ +p_size.x, +p_size.y, -p_size.z }, p_color, p_lineWidth);
	DrawLine(p_pso, p_position + p_rotation * OvMaths::FVector3{ +p_size.x, -p_size.y, +p_size.z }, p_position + p_rotation * OvMaths::FVector3{ +p_size.x, +p_size.y, +p_size.z }, p_color, p_lineWidth);
	DrawLine(p_pso, p_position + p_rotation * OvMaths::FVector3{ -p_size.x, -p_size.y, -p_size.z }, p_position + p_rotation * OvMaths::FVector3{ +p_size.x, -p_size.y, -p_size.z }, p_color, p_lineWidth);
	DrawLine(p_pso, p_position + p_rotation * OvMaths::FVector3{ -p_size.x, +p_size.y, -p_size.z }, p_position + p_rotation * OvMaths::FVector3{ +p_size.x, +p_size.y, -p_size.z }, p_color, p_lineWidth);
	DrawLine(p_pso, p_position + p_rotation * OvMaths::FVector3{ -p_size.x, -p_size.y, +p_size.z }, p_position + p_rotation * OvMaths::FVector3{ +p_size.x, -p_size.y, +p_size.z }, p_color, p_lineWidth);
	DrawLine(p_pso, p_position + p_rotation * OvMaths::FVector3{ -p_size.x, +p_size.y, +p_size.z }, p_position + p_rotation * OvMaths::FVector3{ +p_size.x, +p_size.y, +p_size.z }, p_color, p_lineWidth);
}

void OvRendering::Features::DebugShapeRenderFeature::DrawSphere(OvRendering::Data::PipelineState p_pso, const OvMaths::FVector3& p_position, const OvMaths::FQuaternion& p_rotation, float p_radius, const OvMaths::FVector3& p_color, float p_lineWidth)
{
	if (!std::isinf(p_radius))
	{
		for (float i = 0; i <= 360.0f; i += 10.0f)
		{
			DrawLine(p_pso, p_position + p_rotation * (OvMaths::FVector3{ cos(i * (3.14f / 180.0f)), sin(i * (3.14f / 180.0f)), 0.f } *p_radius), p_position + p_rotation * (OvMaths::FVector3{ cos((i + 10.0f) * (3.14f / 180.0f)), sin((i + 10.0f) * (3.14f / 180.0f)), 0.f } *p_radius), p_color, p_lineWidth);
			DrawLine(p_pso, p_position + p_rotation * (OvMaths::FVector3{ 0.f, sin(i * (3.14f / 180.0f)), cos(i * (3.14f / 180.0f)) } *p_radius), p_position + p_rotation * (OvMaths::FVector3{ 0.f, sin((i + 10.0f) * (3.14f / 180.0f)), cos((i + 10.0f) * (3.14f / 180.0f)) } *p_radius), p_color, p_lineWidth);
			DrawLine(p_pso, p_position + p_rotation * (OvMaths::FVector3{ cos(i * (3.14f / 180.0f)), 0.f, sin(i * (3.14f / 180.0f)) } *p_radius), p_position + p_rotation * (OvMaths::FVector3{ cos((i + 10.0f) * (3.14f / 180.0f)), 0.f, sin((i + 10.0f) * (3.14f / 180.0f)) } *p_radius), p_color, p_lineWidth);
		}
	}
}

void OvRendering::Features::DebugShapeRenderFeature::DrawCapsule(OvRendering::Data::PipelineState p_pso, const OvMaths::FVector3& p_position, const OvMaths::FQuaternion& p_rotation, float p_radius, float p_height, const OvMaths::FVector3& p_color, float p_lineWidth)
{
	if (!std::isinf(p_radius))
	{
		float halfHeight = p_height / 2;

		OvMaths::FVector3 hVec = { 0.0f, halfHeight, 0.0f };

		for (float i = 0; i < 360.0f; i += 10.0f)
		{
			DrawLine(p_pso, p_position + p_rotation * (hVec + OvMaths::FVector3{ cos(i * (3.14f / 180.0f)), 0.f, sin(i * (3.14f / 180.0f)) } *p_radius), p_position + p_rotation * (hVec + OvMaths::FVector3{ cos((i + 10.0f) * (3.14f / 180.0f)), 0.f, sin((i + 10.0f) * (3.14f / 180.0f)) } *p_radius), p_color, p_lineWidth);
			DrawLine(p_pso, p_position + p_rotation * (-hVec + OvMaths::FVector3{ cos(i * (3.14f / 180.0f)), 0.f, sin(i * (3.14f / 180.0f)) } *p_radius), p_position + p_rotation * (-hVec + OvMaths::FVector3{ cos((i + 10.0f) * (3.14f / 180.0f)), 0.f, sin((i + 10.0f) * (3.14f / 180.0f)) } *p_radius), p_color, p_lineWidth);

			if (i < 180.f)
			{
				DrawLine(p_pso, p_position + p_rotation * (hVec + OvMaths::FVector3{ cos(i * (3.14f / 180.0f)), sin(i * (3.14f / 180.0f)), 0.f } *p_radius), p_position + p_rotation * (hVec + OvMaths::FVector3{ cos((i + 10.0f) * (3.14f / 180.0f)), sin((i + 10.0f) * (3.14f / 180.0f)), 0.f } *p_radius), p_color, p_lineWidth);
				DrawLine(p_pso, p_position + p_rotation * (hVec + OvMaths::FVector3{ 0.f, sin(i * (3.14f / 180.0f)), cos(i * (3.14f / 180.0f)) } *p_radius), p_position + p_rotation * (hVec + OvMaths::FVector3{ 0.f, sin((i + 10.0f) * (3.14f / 180.0f)), cos((i + 10.0f) * (3.14f / 180.0f)) } *p_radius), p_color, p_lineWidth);
			}
			else
			{
				DrawLine(p_pso, p_position + p_rotation * (-hVec + OvMaths::FVector3{ cos(i * (3.14f / 180.0f)), sin(i * (3.14f / 180.0f)), 0.f } *p_radius), p_position + p_rotation * (-hVec + OvMaths::FVector3{ cos((i + 10.0f) * (3.14f / 180.0f)), sin((i + 10.0f) * (3.14f / 180.0f)), 0.f } *p_radius), p_color, p_lineWidth);
				DrawLine(p_pso, p_position + p_rotation * (-hVec + OvMaths::FVector3{ 0.f, sin(i * (3.14f / 180.0f)), cos(i * (3.14f / 180.0f)) } *p_radius), p_position + p_rotation * (-hVec + OvMaths::FVector3{ 0.f, sin((i + 10.0f) * (3.14f / 180.0f)), cos((i + 10.0f) * (3.14f / 180.0f)) } *p_radius), p_color, p_lineWidth);
			}
		}

		DrawLine(p_pso, p_position + p_rotation * (OvMaths::FVector3{ -p_radius, -halfHeight, 0.f }), p_position + p_rotation * (OvMaths::FVector3{ -p_radius, +halfHeight, 0.f }), p_color, p_lineWidth);
		DrawLine(p_pso, p_position + p_rotation * (OvMaths::FVector3{ p_radius, -halfHeight, 0.f }), p_position + p_rotation * (OvMaths::FVector3{ p_radius, +halfHeight, 0.f }), p_color, p_lineWidth);
		DrawLine(p_pso, p_position + p_rotation * (OvMaths::FVector3{ 0.f, -halfHeight, -p_radius }), p_position + p_rotation * (OvMaths::FVector3{ 0.f, +halfHeight, -p_radius }), p_color, p_lineWidth);
		DrawLine(p_pso, p_position + p_rotation * (OvMaths::FVector3{ 0.f, -halfHeight, p_radius }), p_position + p_rotation * (OvMaths::FVector3{ 0.f, +halfHeight, p_radius }), p_color, p_lineWidth);
	}
}
