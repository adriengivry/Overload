/**
* @project: Overload
* @author: Overload Tech.
* @licence: MIT
*/

#pragma once

#include <map>

#include <OvRendering/Core/CompositeRenderer.h>
#include <OvRendering/Resources/Mesh.h>
#include <OvRendering/Data/Frustum.h>
#include <OvRendering/Entities/Drawable.h>
#include <OvRendering/Buffers/UniformBuffer.h>
#include <OvRendering/Buffers/ShaderStorageBuffer.h>

#include "OvCore/Resources/Material.h"
#include "OvCore/ECS/Actor.h"
#include "OvCore/ECS/Components/CCamera.h"
#include "OvCore/SceneSystem/Scene.h"

namespace OvCore::Rendering
{
	/**
	* Extension of the ImmediateRenderer adding support for the scene system
	*/
	class SceneRenderer : public OvRendering::Core::CompositeRenderer
	{
	public:
		using OpaqueDrawables		= std::multimap<float, OvRendering::Entities::Drawable, std::less<float>>;
		using TransparentDrawables	= std::multimap<float, OvRendering::Entities::Drawable, std::greater<float>>;

		/**
		* Constructor of the Renderer
		* @param p_driver
		*/
		SceneRenderer(OvRendering::Context::Driver& p_driver);

		/**
		* Destructor of the Renderer
		* @param p_driver
		*/
		virtual ~SceneRenderer() = default;

		/**
		* Render the scene
		* @param p_scene
		* @param p_viewportWidth
		* @param p_viewportHeight
		* @param p_materialOverride
		*/
		void RenderScene(
			OvCore::SceneSystem::Scene& p_scene,
			uint16_t p_viewportWidth,
			uint16_t p_viewportHeight,
			std::optional<std::reference_wrapper<OvCore::Resources::Material>> p_materialOverride = std::nullopt
		);

		/**
		* Parse a scene to find the main camera
		* @param p_scene
		*/
		OvCore::ECS::Components::CCamera* FindMainCamera(const OvCore::SceneSystem::Scene& p_scene);

		/**
		* Update the light SSBO with the current scene
		* @param p_scene
		*/
		void UpdateLights(OvCore::SceneSystem::Scene& p_scene);

		/**
		* Update the light SSBO with the current scene (Lights outside of the given frustum are culled)
		* @param p_scene
		*/
		void UpdateLightsInFrustum(OvCore::SceneSystem::Scene& p_scene, const OvRendering::Data::Frustum& p_frustum);

		/**
		* Returns opaque and transparents drawables from the scene
		* @param p_scene
		* @param p_cameraPosition
		* @param p_frustum
		* @param p_materialOverride
		*/
		std::pair<OpaqueDrawables, TransparentDrawables> FindAndSortDrawables(
			const OvCore::SceneSystem::Scene& p_scene,
			const OvMaths::FVector3& p_cameraPosition,
			std::optional<OvRendering::Data::Frustum> p_frustum = std::nullopt,
			std::optional<std::reference_wrapper<OvCore::Resources::Material>> p_materialOverride = std::nullopt
		);

		/**
		* Draw the model using the given material (The material will be applied to every submeshes of the the model)
		* @param p_model
		* @param p_material
		* @param p_modelMatrix
		* @param p_materialOverride (Used if the given material has no shader attached)
		*/
		void DrawModelWithSingleMaterial(
			OvRendering::Resources::Model& p_model,
			OvCore::Resources::Material& p_material,
			OvMaths::FMatrix4 const* p_modelMatrix,
			std::optional<std::reference_wrapper<OvCore::Resources::Material>> p_materialOverride = std::nullopt
		);

	private:
		/**
		* Draw the given scene using the given default material (optional) if no material found on an actor
		* @param p_scene
		* @param p_camera
		* @param p_frustumOverride
		* @param p_materialOverride
		*/
		void RenderScene(
			OvCore::SceneSystem::Scene& p_scene,
			const OvRendering::Entities::Camera& p_camera,
			std::optional<OvRendering::Data::Frustum> p_frustumOverride = std::nullopt,
			std::optional<std::reference_wrapper<OvCore::Resources::Material>> p_materialOverride = std::nullopt
		);
	};
}
