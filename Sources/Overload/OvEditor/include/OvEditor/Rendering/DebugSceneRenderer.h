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
#include "OvEditor/Core/GizmoBehaviour.h"

#include "OvEditor/Core/Context.h"

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
		struct DebugSceneDescriptor
		{
			OvEditor::Core::EGizmoOperation gizmoOperation;
			OvTools::Utils::OptRef<OvCore::ECS::Actor> highlightedActor;
			OvTools::Utils::OptRef<OvCore::ECS::Actor> selectedActor;
			std::optional<OvEditor::Core::GizmoBehaviour::EDirection> highlightedGizmoDirection;
		};

		/**
		* Constructor of the Renderer
		* @param p_driver
		*/
		DebugSceneRenderer(OvRendering::Context::Driver& p_driver);

	protected:
		void DrawActorDebugElements(OvCore::ECS::Actor& p_actor);

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

		// TODO: Rename all these methods to something like "Draw..."
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

		OvCore::Resources::Material m_emptyMaterial;
		OvCore::Resources::Material m_defaultMaterial;
	};
}