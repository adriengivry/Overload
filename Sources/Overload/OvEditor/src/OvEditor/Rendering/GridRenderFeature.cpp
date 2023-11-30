/**
* @project: Overload
* @author: Overload Tech.
* @licence: MIT
*/

#include <OvRendering/Features/DebugShapeRenderFeature.h>

#include <OvDebug/Assertion.h>

#include "OvEditor/Core/EditorResources.h"

#include "OvEditor/Core/EditorActions.h"
#include "OvEditor/Rendering/GridRenderFeature.h"

OvEditor::Rendering::GridRenderFeature::GridRenderFeature(OvRendering::Core::CompositeRenderer& p_renderer) :
	OvRendering::Features::ARenderFeature(p_renderer)
{
	/* Grid Material */
	m_gridMaterial.SetShader(EDITOR_CONTEXT(editorResources)->GetShader("Grid"));
	m_gridMaterial.SetBlendable(true);
	m_gridMaterial.SetBackfaceCulling(false);
	m_gridMaterial.SetDepthTest(false);
}

void OvEditor::Rendering::GridRenderFeature::DrawPass(OvRendering::Settings::ERenderPass p_pass)
{
	OVASSERT(m_renderer.HasDescriptor<GridDescriptor>(), "Cannot find GridDescriptor attached to this renderer");
	OVASSERT(m_renderer.HasFeature<OvRendering::Features::DebugShapeRenderFeature>(), "Cannot find DebugShapeRenderFeature attached to this renderer");

	auto& gridDescriptor = m_renderer.GetDescriptor<GridDescriptor>();
	auto& debugShapeRenderer = m_renderer.GetFeature<OvRendering::Features::DebugShapeRenderFeature>();

	constexpr float gridSize = 5000.0f;

	OvMaths::FMatrix4 model =
		OvMaths::FMatrix4::Translation({ gridDescriptor.viewPosition.x, 0.0f, gridDescriptor.viewPosition.z }) *
		OvMaths::FMatrix4::Scaling({ gridSize * 2.0f, 1.f, gridSize * 2.0f });

	m_gridMaterial.Set("u_Color", gridDescriptor.gridColor);
	m_renderer.DrawModelWithSingleMaterial(*EDITOR_CONTEXT(editorResources)->GetModel("Plane"), m_gridMaterial, model);

	debugShapeRenderer.DrawLine(OvMaths::FVector3(-gridSize + gridDescriptor.viewPosition.x, 0.0f, 0.0f), OvMaths::FVector3(gridSize + gridDescriptor.viewPosition.x, 0.0f, 0.0f), OvMaths::FVector3(1.0f, 0.0f, 0.0f), 1.0f);
	debugShapeRenderer.DrawLine(OvMaths::FVector3(0.0f, -gridSize + gridDescriptor.viewPosition.y, 0.0f), OvMaths::FVector3(0.0f, gridSize + gridDescriptor.viewPosition.y, 0.0f), OvMaths::FVector3(0.0f, 1.0f, 0.0f), 1.0f);
	debugShapeRenderer.DrawLine(OvMaths::FVector3(0.0f, 0.0f, -gridSize + gridDescriptor.viewPosition.z), OvMaths::FVector3(0.0f, 0.0f, gridSize + gridDescriptor.viewPosition.z), OvMaths::FVector3(0.0f, 0.0f, 1.0f), 1.0f);
}

OvRendering::Settings::ERenderPassMask OvEditor::Rendering::GridRenderFeature::GetRenderPassMask() const
{
	return OvRendering::Settings::ERenderPassMask::PRE_OPAQUE;
}
