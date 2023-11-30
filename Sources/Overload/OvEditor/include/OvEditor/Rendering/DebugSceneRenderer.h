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

namespace OvEditor::Core { enum class EGizmoOperation; }
namespace OvEditor::Panels { class AView; }

namespace OvEditor::Rendering
{
	/**
	* Provide a debug layer on top of the default scene renderer to see "invisible" entities such as
	* lights, cameras, 
	*/
	class DebugSceneRenderer : public OvCore::Rendering::SceneRenderer
	{
	public:
		/**
		* Constructor of the Renderer
		* @param p_driver
		*/
		DebugSceneRenderer(OvRendering::Context::Driver& p_driver);

		/**
		* Add the debug drawing logic on top of SceneRenderer draw method
		* @param p_pass
		*/
		virtual void DrawPass(OvRendering::Settings::ERenderPass p_pass) override;

	protected:
		/**
		* Prepare the picking material by send it the color corresponding to the given actor
		* @param p_actor
		* @param p_material
		*/
		void PreparePickingMaterial(OvCore::ECS::Actor& p_actor, OvCore::Resources::Material& p_material);
		// TODO: Consider moving everything actor picking related to its own render feature

		/**
		* Render the scene for actor picking (Unlit version of the scene with colors indicating actor IDs)
		* @param p_scene
		*/
		void RenderSceneForActorPicking(OvCore::SceneSystem::Scene& p_scene);

		/**
		* Render a model to the stencil buffer
		*/
		void RenderModelToStencil(const OvMaths::FMatrix4& p_worldMatrix, OvRendering::Resources::Model& p_model);

		/**
		* Render a model outline using the data stored in the stencil buffer
		* @param p_worldMatrix
		* @param p_model
		* @param p_width
		*/
		void RenderModelOutline(const OvMaths::FMatrix4& p_worldMatrix, OvRendering::Resources::Model& p_model, float p_width);

		/**
		* Render the actor as a selected actor (Outline)
		* @param p_actor
		* @param p_toStencil
		* @param p_isSelected
		*/
		void RenderActorOutlinePass(OvCore::ECS::Actor& p_actor, bool p_toStencil, bool p_isSelected = false);

		/**
		* Draw frustum lines
		* @param pos
		* @param p_forward
		* @param p_near
		* @param p_far
		* @param p_a
		* @parma p_b
		* @param p_c
		* @param p_d
		* @param p_e
		* @param p_f
		* @param p_g
		* @param p_h
		*/
		void DrawFrustumLines(
			const OvMaths::FVector3& pos,
			const OvMaths::FVector3& forward,
			float near,
			const float far,
			const OvMaths::FVector3& a,
			const OvMaths::FVector3& b,
			const OvMaths::FVector3& c,
			const OvMaths::FVector3& d,
			const OvMaths::FVector3& e,
			const OvMaths::FVector3& f,
			const OvMaths::FVector3& g,
			const OvMaths::FVector3& h
		);

		void RenderCameras(OvCore::SceneSystem::Scene& p_scene);
		void RenderLights(OvCore::SceneSystem::Scene& p_scene);
		void RenderCameraPerspectiveFrustum(std::pair<uint16_t, uint16_t>& p_size, OvCore::ECS::Components::CCamera& p_camera);
		void RenderCameraOrthographicFrustum(std::pair<uint16_t, uint16_t>& p_size, OvCore::ECS::Components::CCamera& p_camera);
		void RenderCameraFrustum(OvCore::ECS::Components::CCamera& p_camera);
		void RenderActorCollider(OvCore::ECS::Actor& p_actor);
		void RenderLightBounds(OvCore::ECS::Components::CLight& p_light);
		void RenderAmbientBoxVolume(OvCore::ECS::Components::CAmbientBoxLight& p_ambientBoxLight);
		void RenderAmbientSphereVolume(OvCore::ECS::Components::CAmbientSphereLight& p_ambientSphereLight);
		void RenderBoundingSpheres(OvCore::ECS::Components::CModelRenderer& p_modelRenderer);

	private:
		OvRendering::Features::DebugShapeRenderFeature& m_debugShapeFeature;

		OvCore::Resources::Material m_stencilFillMaterial;
		OvCore::Resources::Material m_outlineMaterial;
		OvCore::Resources::Material m_emptyMaterial;
		OvCore::Resources::Material m_defaultMaterial;
		OvCore::Resources::Material m_cameraMaterial;
		OvCore::Resources::Material m_lightMaterial;
		OvCore::Resources::Material m_actorPickingMaterial;
	};
}