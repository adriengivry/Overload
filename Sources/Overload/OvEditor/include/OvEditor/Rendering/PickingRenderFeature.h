/**
* @project: Overload
* @author: Overload Tech.
* @licence: MIT
*/

#pragma once

#include <OvRendering/Entities/Camera.h>
#include <OvRendering/Features/DebugShapeRenderFeature.h>

#include <OvCore/ECS/Actor.h>
#include <OvCore/SceneSystem/SceneManager.h>
#include <OvCore/ECS/Components/CModelRenderer.h>
#include <OvCore/Resources/Material.h>
#include <OvCore/ECS/Components/CAmbientBoxLight.h>
#include <OvCore/ECS/Components/CAmbientSphereLight.h>
#include <OvCore/Rendering/SceneRenderer.h>

#include "OvEditor/Core/Context.h"
#include "OvEditor/Core/GizmoBehaviour.h"

namespace OvEditor::Rendering
{
	/**
	* Draw the scene for actor picking
	*/
	class PickingRenderFeature : public OvRendering::Features::ARenderFeature
	{
	public:
		using PickingResult =
			std::optional<
			std::variant<OvTools::Utils::OptRef<OvCore::ECS::Actor>,
			OvEditor::Core::GizmoBehaviour::EDirection>
			>;

		/**
		* Constructor
		* @param p_renderer
		*/
		PickingRenderFeature(OvRendering::Core::CompositeRenderer& p_renderer);

		/**
		* Returns the render pass flags associated with this feature
		*/
		virtual OvRendering::Settings::ERenderPassMask GetRenderPassMask() const;

		/**
		* Draw for the given render pass
		* @param p_renderPass
		*/
		virtual void DrawPass(OvRendering::Settings::ERenderPass p_renderPass);

		/**
		* Return the picking result at the given position
		* @param p_scene
		* @param p_x
		* @param p_y
		*/
		PickingResult ReadbackPickingResult(
			const OvCore::SceneSystem::Scene& p_scene,
			uint32_t p_x,
			uint32_t p_y
		);

	private:
		void DrawPickableModels(OvRendering::Data::PipelineState p_pso, OvCore::SceneSystem::Scene& p_scene);
		void DrawPickableCameras(OvRendering::Data::PipelineState p_pso, OvCore::SceneSystem::Scene& p_scene);
		void DrawPickableLights(OvRendering::Data::PipelineState p_pso, OvCore::SceneSystem::Scene& p_scene);
		void DrawPickableGizmo(
			OvRendering::Data::PipelineState p_pso,
			const OvMaths::FVector3& p_position,
			const OvMaths::FQuaternion& p_rotation,
			OvEditor::Core::EGizmoOperation p_operation
		);

	private:
		OvRendering::Buffers::Framebuffer m_actorPickingFramebuffer;
		OvCore::Resources::Material m_actorPickingMaterial;
		OvCore::Resources::Material m_lightMaterial;
		OvCore::Resources::Material m_gizmoPickingMaterial;
	};
}