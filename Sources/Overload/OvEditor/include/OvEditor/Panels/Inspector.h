/**
* @project: Overload
* @author: Overload Tech.
* @licence: MIT
*/

#pragma once

#include <OvRendering/Resources/Loaders/TextureLoader.h>

#include <OvUI/Panels/PanelWindow.h>
#include <OvUI/Widgets/Layout/GroupCollapsable.h>
#include <OvUI/Widgets/InputFields/InputText.h>
#include <OvUI/Widgets/Visual/Separator.h>
#include <OvUI/Widgets/Selection/CheckBox.h>
#include <OvUI/Widgets/Buttons/Button.h>
#include <OvUI/Widgets/Selection/ComboBox.h>

#include "OvEditor/Panels/Hierarchy.h"
#include "OvEditor/Panels/AssetBrowser.h"

namespace OvEditor::Panels
{
	class Inspector : public OvUI::Panels::PanelWindow
	{
	public:
		/**
		* Constructor
		* @param p_title
		* @param p_opened
		* @param p_windowSettings
		*/
		Inspector
		(
			const std::string& p_title,
			bool p_opened,
			const OvUI::Settings::PanelWindowSettings& p_windowSettings
		);

		/**
		* Destructor
		*/
		~Inspector();

		/**
		* Focus the given actor
		* @param p_target
		*/
		void FocusActor(OvCore::ECS::Actor& p_target);

		/**
		* Unfocus the currently targeted actor
		*/
		void UnFocus();

		/**
		* Unfocus the currently targeted actor without removing listeners attached to this actor
		*/
		void SoftUnFocus();

		/**
		* Returns the currently selected actor
		*/
		OvCore::ECS::Actor* GetTargetActor() const;

		/**
		* Create the actor inspector for the given actor
		*/
		void CreateActorInspector(OvCore::ECS::Actor& p_target);

		/**
		* Draw the given component in inspector
		*/
		void DrawComponent(OvCore::ECS::Components::AComponent& p_component);

		/**
		* Draw the given behaviour in inspector
		*/
		void DrawBehaviour(OvCore::ECS::Components::Behaviour& p_behaviour);

		/**
		* Refresh the inspector
		*/
		void Refresh();

	private:
		OvCore::ECS::Actor* m_targetActor = nullptr;
		OvUI::Widgets::Layout::Group* m_actorInfo;
		OvUI::Widgets::Layout::Group* m_inspectorHeader;
		OvUI::Widgets::Selection::ComboBox* m_componentSelectorWidget;
        OvUI::Widgets::InputFields::InputText* m_scriptSelectorWidget;

		uint64_t m_componentAddedListener	= 0;
		uint64_t m_componentRemovedListener = 0;
		uint64_t m_behaviourAddedListener	= 0;
		uint64_t m_behaviourRemovedListener = 0;
		uint64_t m_destroyedListener		= 0;
	};
}