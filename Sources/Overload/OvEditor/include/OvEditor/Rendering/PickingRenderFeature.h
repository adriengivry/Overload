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

namespace OvEditor::Rendering
{
	/**
	* Draw the scene for actor picking
	*/
	class PickingRenderFeature : public OvRendering::Features::ARenderFeature
	{
	public:
		/**
		* Constructor
		* @param p_renderer
		*/
		PickingRenderFeature(OvCore::Rendering::SceneRenderer& p_renderer);

		/**
		* Returns the render pass flags associated with this feature
		*/
		virtual OvRendering::Settings::ERenderPassMask GetRenderPassMask() const;

		/**
		* Draw for the given render pass
		* @param p_renderPass
		*/
		virtual void DrawPass(OvRendering::Settings::ERenderPass p_renderPass);

	private:
		void DrawPickableModels(OvCore::SceneSystem::Scene& p_scene);
		void DrawPickableCameras(OvCore::SceneSystem::Scene& p_scene);
		void DrawPickableLights(OvCore::SceneSystem::Scene& p_scene);

	private:
		OvRendering::Buffers::Framebuffer m_actorPickingFramebuffer;
		OvCore::Resources::Material m_actorPickingMaterial;
		OvCore::Resources::Material m_emptyMaterial;
		OvCore::Resources::Material m_lightMaterial;
	};
}