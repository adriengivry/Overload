/**
* @project: Overload
* @author: Overload Tech.
* @licence: MIT
*/

#include "OvEditor/Rendering/DebugModelRenderFeature.h"
#include "OvEditor/Rendering/ShadowRenderFeature.h"
#include "OvEditor/Rendering/ShadowRenderPass.h"
#include "OvEditor/Core/EditorActions.h"
#include "OvEditor/Settings/EditorSettings.h"
#include "OvEditor/Rendering/DebugSceneRenderer.h"

#include <OvCore/ECS/Components/CMaterialRenderer.h>
#include <OvCore/Rendering/EngineDrawableDescriptor.h>
#include <OvCore/Rendering/EngineBufferRenderFeature.h>
#include <OvRendering/Features/LightingRenderFeature.h>

constexpr uint16_t kShadowMapSize = 1024;

OvEditor::Rendering::ShadowRenderFeature::ShadowRenderFeature(OvRendering::Core::CompositeRenderer& p_renderer) :
	ARenderFeature(p_renderer)
{
}

void OvEditor::Rendering::ShadowRenderFeature::OnBeforeDraw(OvRendering::Data::PipelineState& p_pso, const OvRendering::Entities::Drawable& p_drawable)
{
	const float near_plane = 1.0f, far_plane = 7.5f;
	OvRendering::Entities::Camera lightCamera;
	const auto pos = OvMaths::FVector3{ 1.0f, 10.0f, 1.0f };
	lightCamera.SetPosition(pos);
	lightCamera.SetRotation(OvMaths::FQuaternion::LookAt(OvMaths::FVector3::Zero - pos, OvMaths::FVector3::Up));
	lightCamera.CacheMatrices(kShadowMapSize, kShadowMapSize);
	const auto lightSpaceMatrix = lightCamera.GetProjectionMatrix() * lightCamera.GetViewMatrix();

	auto shadowMap = m_renderer.GetPass<ShadowRenderPass>("Shadows").GetDepthMap();
	p_drawable.material.value().Set("u_ShadowMap", shadowMap);
	p_drawable.material.value().Set("u_LightSpaceMatrix", lightSpaceMatrix);
}

void OvEditor::Rendering::ShadowRenderFeature::OnAfterDraw(OvRendering::Data::PipelineState& p_pso, const OvRendering::Entities::Drawable& p_drawable)
{

}

