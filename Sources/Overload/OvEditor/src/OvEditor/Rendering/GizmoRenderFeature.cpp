/**
* @project: Overload
* @author: Overload Tech.
* @licence: MIT
*/

#include <OvCore/ECS/Components/CCamera.h>
#include <OvCore/ECS/Components/CPhysicalBox.h>
#include <OvCore/ECS/Components/CPhysicalSphere.h>
#include <OvCore/ECS/Components/CPhysicalCapsule.h>
#include <OvCore/ECS/Components/CMaterialRenderer.h>
#include <OvCore/ECS/Components/CPointLight.h>
#include <OvCore/ECS/Components/CDirectionalLight.h>
#include <OvCore/ECS/Components/CSpotLight.h>

#include <OvRendering/Features/DebugShapeRenderFeature.h>

#include <OvAnalytics/Profiling/ProfilerSpy.h>

#include <OvDebug/Assertion.h>

#include "OvEditor/Rendering/DebugSceneRenderer.h"
#include "OvEditor/Core/EditorResources.h"
#include "OvEditor/Panels/AView.h"
#include "OvEditor/Settings/EditorSettings.h"
#include "OvEditor/Core/GizmoBehaviour.h"
#include "OvEditor/Core/EditorActions.h"
#include "OvEditor/Rendering/GizmoRenderFeature.h"

OvEditor::Rendering::GizmoRenderFeature::GizmoRenderFeature(OvRendering::Core::CompositeRenderer& p_renderer) :
	OvRendering::Features::ARenderFeature(p_renderer)
{
	/* Gizmo Arrow Material */
	m_gizmoArrowMaterial.SetShader(EDITOR_CONTEXT(editorResources)->GetShader("Gizmo"));
	m_gizmoArrowMaterial.SetGPUInstances(3);
	m_gizmoArrowMaterial.Set("u_IsBall", false);
	m_gizmoArrowMaterial.Set("u_IsPickable", false);

	/* Gizmo Ball Material */
	m_gizmoBallMaterial.SetShader(EDITOR_CONTEXT(editorResources)->GetShader("Gizmo"));
	m_gizmoBallMaterial.Set("u_IsBall", true);
	m_gizmoBallMaterial.Set("u_IsPickable", false);

	/* Gizmo Pickable Material */
	m_gizmoPickingMaterial.SetShader(EDITOR_CONTEXT(editorResources)->GetShader("Gizmo"));
	m_gizmoPickingMaterial.SetGPUInstances(3);
	m_gizmoPickingMaterial.Set("u_IsBall", false);
	m_gizmoPickingMaterial.Set("u_IsPickable", true);
}

void OvEditor::Rendering::GizmoRenderFeature::DrawGizmo(
	const OvMaths::FVector3& p_position,
	const OvMaths::FQuaternion& p_rotation,
	OvEditor::Core::EGizmoOperation p_operation,
	bool p_pickable,
	int p_highlightedAxis
)
{
	using namespace OvMaths;

	FMatrix4 model = FMatrix4::Translation(p_position) * FQuaternion::ToMatrix4(FQuaternion::Normalize(p_rotation));

	OvRendering::Resources::Model* arrowModel = nullptr;

	if (!p_pickable)
	{
		FMatrix4 sphereModel = model * OvMaths::FMatrix4::Scaling({ 0.1f, 0.1f, 0.1f });

		m_renderer.DrawModelWithSingleMaterial(
			*EDITOR_CONTEXT(editorResources)->GetModel("Sphere"),
			m_gizmoBallMaterial,
			sphereModel
		);

		m_gizmoArrowMaterial.Set("u_HighlightedAxis", p_highlightedAxis);

		switch (p_operation)
		{
		case OvEditor::Core::EGizmoOperation::TRANSLATE:
			arrowModel = EDITOR_CONTEXT(editorResources)->GetModel("Arrow_Translate");
			break;
		case OvEditor::Core::EGizmoOperation::ROTATE:
			arrowModel = EDITOR_CONTEXT(editorResources)->GetModel("Arrow_Rotate");
			break;
		case OvEditor::Core::EGizmoOperation::SCALE:
			arrowModel = EDITOR_CONTEXT(editorResources)->GetModel("Arrow_Scale");
			break;
		}
	}
	else
	{
		arrowModel = EDITOR_CONTEXT(editorResources)->GetModel("Arrow_Picking");
	}

	if (arrowModel)
	{
		m_renderer.DrawModelWithSingleMaterial(*arrowModel, p_pickable ? m_gizmoPickingMaterial : m_gizmoArrowMaterial, model);
	}
}
