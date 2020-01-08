/**
* @project: Overload
* @author: Overload Tech.
* @licence: MIT
*/

#pragma once

#include <OvCore/ECS/Actor.h>
#include <OvCore/SceneSystem/SceneManager.h>
#include <OvCore/ECS/Components/CCamera.h>

#include "OvGame/Core/Context.h"

namespace OvGame::Core
{
	/**
	* Handle the rendering of the game scene
	*/
	class GameRenderer
	{
	public:
		/**
		* Create the EditorRenderer
		* @param p_context
		*/
		GameRenderer(Context& p_context);

		/**
		* Render the scene
		*/
		void RenderScene();

		/**
		* Update the engine UBO
		* @param p_mainCamera
		*/
		void UpdateEngineUBO(OvCore::ECS::Components::CCamera& p_mainCamera);

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

	private:
		Context& m_context;
		OvCore::Resources::Material m_emptyMaterial;
	};
}