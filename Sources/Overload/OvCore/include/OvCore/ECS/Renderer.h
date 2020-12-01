/**
* @project: Overload
* @author: Overload Tech.
* @licence: MIT
*/

#pragma once

#include <map>

#include <OvRendering/Core/Renderer.h>
#include <OvRendering/Resources/Mesh.h>
#include <OvRendering/Data/Frustum.h>


#include "OvCore/Resources/Material.h"
#include "OvCore/ECS/Actor.h"
#include "OvCore/ECS/Components/CCamera.h"
#include "OvCore/SceneSystem/Scene.h"

namespace OvCore::ECS
{
	/**
	* A Renderer capable of rendering stuffs linked with the ECS. It is a convenient class that should be used instead of OvRendering::Core::Renderer
	* when you plan to use the OvCore ECS architecture.
	*/
	class Renderer : public OvRendering::Core::Renderer
	{
	public:
		using Drawable				= std::tuple<OvMaths::FMatrix4, OvRendering::Resources::Mesh*, OvCore::Resources::Material*, OvMaths::FMatrix4>;
		using OpaqueDrawables		= std::multimap<float, Drawable, std::less<float>>;
		using TransparentDrawables	= std::multimap<float, Drawable, std::greater<float>>;

		/**
		* Constructor of the Renderer
		* @param p_driver
		*/
		Renderer(OvRendering::Context::Driver& p_driver);

		/**
		* Destructor of the Renderer
		* @param p_driver
		*/
		~Renderer();

		/**
		* Parse a scene to find the main camera
		* @param p_scene
		*/
		OvCore::ECS::Components::CCamera* FindMainCamera(const OvCore::SceneSystem::Scene& p_scene);

		/**
		* Fill the given FMatrix4 vector with lights information
		* @param p_scene
		*/
		std::vector<OvMaths::FMatrix4> FindLightMatrices(const OvCore::SceneSystem::Scene& p_scene);

		/**
		* Fill the given FMatrix4 vector with lights information that are inside the frustum
		* @param p_scene
		* @param p_frustum
		*/
		std::vector<OvMaths::FMatrix4> FindLightMatricesInFrustum(const OvCore::SceneSystem::Scene& p_scene, const OvRendering::Data::Frustum& p_frustum);

		/**
		* Draw the given scene using the given default material (optional) if no material found on an actor
		* @param p_scene
		* @param p_cameraPosition
		* @param p_camera
		* @param p_customFrustum
		* @param p_defaultMaterial
		*/
		void RenderScene
		(
			OvCore::SceneSystem::Scene& p_scene,
			const OvMaths::FVector3& p_cameraPosition,
			const OvRendering::LowRenderer::Camera& p_camera,
			const OvRendering::Data::Frustum* p_customFrustum = nullptr,
			OvCore::Resources::Material* p_defaultMaterial = nullptr
		);

		/**
		* Returns opaque and transparents drawables from the scene with frustum culling
		* @param p_scene
		* @param p_cameraPosition
		* @param p_frustum
		* @param p_defaultMaterial
		*/
		std::pair<OpaqueDrawables, TransparentDrawables> FindAndSortFrustumCulledDrawables
		(
			const OvCore::SceneSystem::Scene& p_scene,
			const OvMaths::FVector3& p_cameraPosition,
			const OvRendering::Data::Frustum& p_frustum,
			OvCore::Resources::Material* p_defaultMaterial
		);

		/**
		* Returns opaque and transparents drawables from the scene
		* @param p_scene
		* @param p_cameraPosition
		* @param p_defaultMaterial
		*/
		std::pair<OpaqueDrawables, TransparentDrawables> FindAndSortDrawables
		(
			const OvCore::SceneSystem::Scene& p_scene,
			const OvMaths::FVector3& p_cameraPosition,
			OvCore::Resources::Material* p_defaultMaterial
		);

		/**
		* Draw a Drawable instance
		* @param p_toDraw
		*/
		void DrawDrawable(const Drawable& p_toDraw);

		/**
		* Draw the model using the given material (The material will be applied to every submeshes of the the model)
		* @param p_model
		* @param p_material
		* @param p_modelMatrix
		* @param p_defaultMaterial (Used if the given material has no shader attached)
		*/
		void DrawModelWithSingleMaterial(OvRendering::Resources::Model& p_model, OvCore::Resources::Material& p_material, OvMaths::FMatrix4 const* p_modelMatrix, OvCore::Resources::Material* p_defaultMaterial = nullptr);

		/**
		* Draw the model using the given materials
		* @param p_model
		* @param p_modelMatrix
		* @param p_defaultMaterial (Used when a submesh material has no shader attached)
		*/
		void DrawModelWithMaterials(OvRendering::Resources::Model& p_model, std::vector<OvCore::Resources::Material*> p_materials, OvMaths::FMatrix4 const* p_modelMatrix, OvCore::Resources::Material* p_defaultMaterial = nullptr);

		/**
		* Try drawing a mesh using the given material (Fails if the material has no shader attached)
		* @param p_mesh
		* @param p_material
		* @param p_modelMatrix (If set to nullptr, no data will be sent to the GPU)
		*/
		void DrawMesh(OvRendering::Resources::Mesh& p_mesh, OvCore::Resources::Material& p_material, OvMaths::FMatrix4 const* p_modelMatrix);

		/**
		* Register the given function as the model matrix sender.
		* The given function will be called before every draw calls.
		* You should use this function to send the model matrix
		* to the GPU in the way you want
		* @param p_modelMatrixSender
		*/
		void RegisterModelMatrixSender(std::function<void(OvMaths::FMatrix4)> p_modelMatrixSender);

		/**
		* Register the given function as the user matrix sender.
		* The given function will be called before every draw calls.
		* You should use this function to send the user matrix
		* to the GPU in the way you want
		* @param p_userMatrixSender
		*/
		void RegisterUserMatrixSender(std::function<void(OvMaths::FMatrix4)> p_userMatrixSender);

	private:
		std::function<void(OvMaths::FMatrix4)> m_modelMatrixSender;
		std::function<void(OvMaths::FMatrix4)> m_userMatrixSender;
		OvRendering::Resources::Texture* m_emptyTexture = nullptr;
	};
}