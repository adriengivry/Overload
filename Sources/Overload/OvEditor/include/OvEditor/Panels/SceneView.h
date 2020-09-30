/**
* @project: Overload
* @author: Overload Tech.
* @licence: MIT
*/

#pragma once

#include "OvEditor/Panels/AViewControllable.h"
#include "OvEditor/Core/GizmoBehaviour.h"

namespace OvEditor::Panels
{
	class SceneView : public OvEditor::Panels::AViewControllable
	{
	public:
		/**
		* Constructor
		* @param p_title
		* @param p_opened
		* @param p_windowSettings
		*/
		SceneView
		(
			const std::string& p_title,
			bool p_opened,
			const OvUI::Settings::PanelWindowSettings& p_windowSettings
		);

		/**
		* Update the scene view
		*/
		virtual void Update(float p_deltaTime) override;

		/**
		* Custom implementation of the render method
		*/
		virtual void _Render_Impl() override;

		/**
		* Render the actual scene
		* @param p_defaultRenderState
		*/
		void RenderScene(uint8_t p_defaultRenderState);

		/**
		* Render the scene for actor picking (Using unlit colors)
		*/
		void RenderSceneForActorPicking();

		/**
		* Render the scene for actor picking and handle the logic behind it
		*/
		void HandleActorPicking();

	private:
		OvCore::SceneSystem::SceneManager& m_sceneManager;
		OvRendering::Buffers::Framebuffer m_actorPickingFramebuffer;
		OvEditor::Core::GizmoBehaviour m_gizmoOperations;
		OvEditor::Core::EGizmoOperation m_currentOperation = OvEditor::Core::EGizmoOperation::TRANSLATE;

		std::optional<std::reference_wrapper<OvCore::ECS::Actor>> m_highlightedActor;
		std::optional<OvEditor::Core::GizmoBehaviour::EDirection> m_highlightedGizmoDirection;
	};
}