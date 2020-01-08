/**
* @project: Overload
* @author: Overload Tech.
* @licence: MIT
*/

#include "OvEditor/Panels/Hierarchy.h"
#include "OvEditor/Core/EditorActions.h"

#include <OvUI/Widgets/Buttons/Button.h>
#include <OvUI/Widgets/Selection/CheckBox.h>
#include <OvUI/Widgets/Visual/Separator.h>
#include <OvUI/Plugins/DDSource.h>
#include <OvUI/Plugins/DDTarget.h>

#include <OvDebug/Utils/Logger.h>

#include <OvCore/Global/ServiceLocator.h>

#include <OvCore/ECS/Components/CCamera.h>
#include <OvCore/ECS/Components/CPointLight.h>
#include <OvCore/ECS/Components/CDirectionalLight.h>
#include <OvCore/ECS/Components/CSpotLight.h>
#include <OvCore/ECS/Components/CAmbientBoxLight.h>
#include <OvCore/ECS/Components/CAmbientSphereLight.h>
#include <OvCore/ECS/Components/CPhysicalBox.h>
#include <OvCore/ECS/Components/CPhysicalSphere.h>
#include <OvCore/ECS/Components/CPhysicalCapsule.h>
#include <OvCore/ECS/Components/CAudioSource.h>
#include <OvCore/ECS/Components/CAudioListener.h>

#include <OvUI/Plugins/ContextualMenu.h>

class HierarchyActorContextualMenu : public OvUI::Plugins::ContextualMenu
{
public:
	HierarchyActorContextualMenu(OvCore::ECS::Actor& p_target, OvUI::Widgets::Layout::TreeNode& p_treeNode) :
		m_target(p_target), m_treeNode(p_treeNode)
	{
		using namespace OvUI::Panels;
		using namespace OvUI::Widgets;
		using namespace OvUI::Widgets::Menu;
		using namespace OvCore::ECS::Components;

		auto& focusButton = CreateWidget<OvUI::Widgets::Menu::MenuItem>("Focus");
		focusButton.ClickedEvent += [this]
		{
			EDITOR_EXEC(MoveToTarget(m_target));
		};

		auto& duplicateButton = CreateWidget<OvUI::Widgets::Menu::MenuItem>("Duplicate");
		duplicateButton.ClickedEvent += [this]
		{
			EDITOR_EXEC(DelayAction(EDITOR_BIND(DuplicateActor, std::ref(m_target), nullptr, true), 0));
		};

		auto& deleteButton = CreateWidget<OvUI::Widgets::Menu::MenuItem>("Delete");
		deleteButton.ClickedEvent += [this]
		{
			EDITOR_EXEC(DestroyActor(std::ref(m_target)));
		};

		auto& createActor = CreateWidget<OvUI::Widgets::Menu::MenuList>("Create...");

		auto openParent = [this] {m_treeNode.Open(); };

		auto& createEmpty = createActor.CreateWidget<MenuItem>("Create Empty");
		createEmpty.ClickedEvent += EDITOR_BIND(CreateEmptyActor, true, &m_target);
		createEmpty.ClickedEvent += openParent;

		auto& primitives = createActor.CreateWidget<MenuList>("Primitives");
		std::string modelsPath = ":Models\\";
		auto& createCube = primitives.CreateWidget<MenuItem>("Cube");
		createCube.ClickedEvent += EDITOR_BIND(CreateActorWithModel, modelsPath + "Cube.fbx", true, &m_target);
		createCube.ClickedEvent += openParent;

		auto& createSphere = primitives.CreateWidget<MenuItem>("Sphere");
		createSphere.ClickedEvent += EDITOR_BIND(CreateActorWithModel, modelsPath + "Sphere.fbx", true, &m_target);
		createSphere.ClickedEvent += openParent;

		auto& createCone = primitives.CreateWidget<MenuItem>("Cone");
		createCone.ClickedEvent += EDITOR_BIND(CreateActorWithModel, modelsPath + "Cone.fbx", true, &m_target);
		createCone.ClickedEvent += openParent;

		auto& createCylinder = primitives.CreateWidget<MenuItem>("Cylinder");
		createCylinder.ClickedEvent += EDITOR_BIND(CreateActorWithModel, modelsPath + "Cylinder.fbx", true, &m_target);
		createCylinder.ClickedEvent += openParent;

		auto& createPlane = primitives.CreateWidget<MenuItem>("Plane");
		createPlane.ClickedEvent += EDITOR_BIND(CreateActorWithModel, modelsPath + "Plane.fbx", true, &m_target);
		createPlane.ClickedEvent += openParent;

		auto& createGear = primitives.CreateWidget<MenuItem>("Gear");
		createGear.ClickedEvent += EDITOR_BIND(CreateActorWithModel, modelsPath + "Gear.fbx", true, &m_target);
		createGear.ClickedEvent += openParent;

		auto& createHelix = primitives.CreateWidget<MenuItem>("Helix");
		createHelix.ClickedEvent += EDITOR_BIND(CreateActorWithModel, modelsPath + "Helix.fbx", true, &m_target);
		createHelix.ClickedEvent += openParent;

		auto& createPipe = primitives.CreateWidget<MenuItem>("Pipe");
		createPipe.ClickedEvent += EDITOR_BIND(CreateActorWithModel, modelsPath + "Pipe.fbx", true, &m_target);
		createPipe.ClickedEvent += openParent;

		auto& createPyramid = primitives.CreateWidget<MenuItem>("Pyramid");
		createPyramid.ClickedEvent += EDITOR_BIND(CreateActorWithModel, modelsPath + "Pyramid.fbx", true, &m_target);
		createPyramid.ClickedEvent += openParent;

		auto& createTorus = primitives.CreateWidget<MenuItem>("Torus");
		createTorus.ClickedEvent += EDITOR_BIND(CreateActorWithModel, modelsPath + "Torus.fbx", true, &m_target);
		createTorus.ClickedEvent += openParent;

		auto& physicals = createActor.CreateWidget<MenuList>("Physicals");

		auto& createPhysicalBox = physicals.CreateWidget<MenuItem>("Physical Box");
		createPhysicalBox.ClickedEvent += EDITOR_BIND(CreatePhysicalBox, true, &m_target);
		createPhysicalBox.ClickedEvent += openParent;

		auto& createPhysicalSphere = physicals.CreateWidget<MenuItem>("Physical Sphere");
		createPhysicalSphere.ClickedEvent += EDITOR_BIND(CreatePhysicalSphere, true, &m_target);
		createPhysicalSphere.ClickedEvent += openParent;

		auto& createPhysicalCapsule = physicals.CreateWidget<MenuItem>("Physical Capsule");
		createPhysicalCapsule.ClickedEvent += EDITOR_BIND(CreatePhysicalCapsule, true, &m_target);
		createPhysicalCapsule.ClickedEvent += openParent;

		auto& lights = createActor.CreateWidget<MenuList>("Lights");

		auto& createPoint = lights.CreateWidget<MenuItem>("Point");
		createPoint.ClickedEvent += EDITOR_BIND(CreateMonoComponentActor<CPointLight>, true, &m_target);
		createPoint.ClickedEvent += openParent;

		auto& createDirectional = lights.CreateWidget<MenuItem>("Directional");
		createDirectional.ClickedEvent += EDITOR_BIND(CreateMonoComponentActor<CDirectionalLight>, true, &m_target);
		createDirectional.ClickedEvent += openParent;

		auto& createSpot = lights.CreateWidget<MenuItem>("Spot");
		createSpot.ClickedEvent += EDITOR_BIND(CreateMonoComponentActor<CSpotLight>, true, &m_target);
		createSpot.ClickedEvent += openParent;

		auto& createAmbientBox = lights.CreateWidget<MenuItem>("Ambient Box");
		createAmbientBox.ClickedEvent += EDITOR_BIND(CreateMonoComponentActor<CAmbientBoxLight>, true, &m_target);
		createAmbientBox.ClickedEvent += openParent;

		auto& createAmbientSphere = lights.CreateWidget<MenuItem>("Ambient Sphere");
		createAmbientSphere.ClickedEvent += EDITOR_BIND(CreateMonoComponentActor<CAmbientSphereLight>, true, &m_target);
		createAmbientSphere.ClickedEvent += openParent;

		auto& audio = createActor.CreateWidget<MenuList>("Audio");

		auto& createAudioSource = audio.CreateWidget<MenuItem>("Audio Source");
		createAudioSource.ClickedEvent += EDITOR_BIND(CreateMonoComponentActor<CAudioSource>, true, &m_target);
		createAudioSource.ClickedEvent += openParent;

		auto& createAudioListener = audio.CreateWidget<MenuItem>("Audio Listener");
		createAudioListener.ClickedEvent += EDITOR_BIND(CreateMonoComponentActor<CAudioListener>, true, &m_target);
		createAudioListener.ClickedEvent += openParent;

		auto& others = createActor.CreateWidget<MenuList>("Others");

		auto& createCamera = others.CreateWidget<MenuItem>("Camera");
		createCamera.ClickedEvent += EDITOR_BIND(CreateMonoComponentActor<CCamera>, true, &m_target);
		createCamera.ClickedEvent += openParent;
	}

	virtual void Execute() override
	{
		if (m_widgets.size() > 0)
			OvUI::Plugins::ContextualMenu::Execute();
	}

private:
	OvCore::ECS::Actor& m_target;
	OvUI::Widgets::Layout::TreeNode& m_treeNode;
};

void ExpandTreeNode(OvUI::Widgets::Layout::TreeNode& p_toExpand, const OvUI::Widgets::Layout::TreeNode* p_root)
{
	p_toExpand.Open();

	if (&p_toExpand != p_root && p_toExpand.HasParent())
	{
		ExpandTreeNode(*static_cast<OvUI::Widgets::Layout::TreeNode*>(p_toExpand.GetParent()), p_root);
	}
}

std::vector<OvUI::Widgets::Layout::TreeNode*> nodesToCollapse;
std::vector<OvUI::Widgets::Layout::TreeNode*> founds;

void ExpandTreeNodeAndEnable(OvUI::Widgets::Layout::TreeNode& p_toExpand, const OvUI::Widgets::Layout::TreeNode* p_root)
{
	if (!p_toExpand.IsOpened())
	{
		p_toExpand.Open();
		nodesToCollapse.push_back(&p_toExpand);
	}

	p_toExpand.enabled = true;

	if (&p_toExpand != p_root && p_toExpand.HasParent())
	{
		ExpandTreeNodeAndEnable(*static_cast<OvUI::Widgets::Layout::TreeNode*>(p_toExpand.GetParent()), p_root);
	}
}

OvEditor::Panels::Hierarchy::Hierarchy
(
	const std::string & p_title,
	bool p_opened,
	const OvUI::Settings::PanelWindowSettings& p_windowSettings
) : PanelWindow(p_title, p_opened, p_windowSettings)
{
	auto& searchBar = CreateWidget<OvUI::Widgets::InputFields::InputText>();
	searchBar.ContentChangedEvent += [this](const std::string& p_content)
	{
		founds.clear();
		auto content = p_content;
		std::transform(content.begin(), content.end(), content.begin(), ::tolower);

		for (auto& [actor, item] : m_widgetActorLink)
		{
			if (!p_content.empty())
			{
				auto itemName = item->name;
				std::transform(itemName.begin(), itemName.end(), itemName.begin(), ::tolower);

				if (itemName.find(content) != std::string::npos)
				{
					founds.push_back(item);
				}

				item->enabled = false;
			}
			else
			{
				item->enabled = true;
			}
		}

		for (auto node : founds)
		{
			node->enabled = true;

			if (node->HasParent())
			{
				ExpandTreeNodeAndEnable(*static_cast<OvUI::Widgets::Layout::TreeNode*>(node->GetParent()), m_sceneRoot);
			}
		}

		if (p_content.empty())
		{
			for (auto node : nodesToCollapse)
			{
				node->Close();
			}

			nodesToCollapse.clear();
		}
	};

	m_sceneRoot = &CreateWidget<OvUI::Widgets::Layout::TreeNode>("Root", true);
	static_cast<OvUI::Widgets::Layout::TreeNode*>(m_sceneRoot)->Open();
	m_sceneRoot->AddPlugin<OvUI::Plugins::DDTarget<std::pair<OvCore::ECS::Actor*, OvUI::Widgets::Layout::TreeNode*>>>("Actor").DataReceivedEvent += [this](std::pair<OvCore::ECS::Actor*, OvUI::Widgets::Layout::TreeNode*> p_element)
	{
		if (p_element.second->HasParent())
			p_element.second->GetParent()->UnconsiderWidget(*p_element.second);

		m_sceneRoot->ConsiderWidget(*p_element.second);

		p_element.first->DetachFromParent();
	};

	EDITOR_EVENT(ActorUnselectedEvent) += std::bind(&Hierarchy::UnselectActorsWidgets, this);
	EDITOR_CONTEXT(sceneManager).SceneUnloadEvent += std::bind(&Hierarchy::Clear, this);
	OvCore::ECS::Actor::CreatedEvent += std::bind(&Hierarchy::AddActorByInstance, this, std::placeholders::_1);
	OvCore::ECS::Actor::DestroyedEvent += std::bind(&Hierarchy::DeleteActorByInstance, this, std::placeholders::_1);
	EDITOR_EVENT(ActorSelectedEvent) += std::bind(&Hierarchy::SelectActorByInstance, this, std::placeholders::_1);
	OvCore::ECS::Actor::AttachEvent += std::bind(&Hierarchy::AttachActorToParent, this, std::placeholders::_1);
	OvCore::ECS::Actor::DettachEvent += std::bind(&Hierarchy::DetachFromParent, this, std::placeholders::_1);
}

void OvEditor::Panels::Hierarchy::Clear()
{
	EDITOR_EXEC(UnselectActor());

	m_sceneRoot->RemoveAllWidgets();
	m_widgetActorLink.clear();
}

void OvEditor::Panels::Hierarchy::UnselectActorsWidgets()
{
	for (auto& widget : m_widgetActorLink)
		widget.second->selected = false;
}

void OvEditor::Panels::Hierarchy::SelectActorByInstance(OvCore::ECS::Actor& p_actor)
{
	if (auto result = m_widgetActorLink.find(&p_actor); result != m_widgetActorLink.end())
		if (result->second)
			SelectActorByWidget(*result->second);
}

void OvEditor::Panels::Hierarchy::SelectActorByWidget(OvUI::Widgets::Layout::TreeNode & p_widget)
{
	UnselectActorsWidgets();

	p_widget.selected = true;

	if (p_widget.HasParent())
	{
		ExpandTreeNode(*static_cast<OvUI::Widgets::Layout::TreeNode*>(p_widget.GetParent()), m_sceneRoot);
	}
}

void OvEditor::Panels::Hierarchy::AttachActorToParent(OvCore::ECS::Actor & p_actor)
{
	auto actorWidget = m_widgetActorLink.find(&p_actor);

	if (actorWidget != m_widgetActorLink.end())
	{
		auto widget = actorWidget->second;

		if (widget->HasParent())
			widget->GetParent()->UnconsiderWidget(*widget);

		if (p_actor.HasParent())
		{
			auto parentWidget = m_widgetActorLink.at(p_actor.GetParent());
			parentWidget->leaf = false;
			parentWidget->ConsiderWidget(*widget);
		}
	}
}

void OvEditor::Panels::Hierarchy::DetachFromParent(OvCore::ECS::Actor & p_actor)
{
	if (auto actorWidget = m_widgetActorLink.find(&p_actor); actorWidget != m_widgetActorLink.end())
	{
		if (p_actor.HasParent() && p_actor.GetParent()->GetChildren().size() == 1)
		{
			if (auto parentWidget = m_widgetActorLink.find(p_actor.GetParent()); parentWidget != m_widgetActorLink.end())
			{
				parentWidget->second->leaf = true;
			}
		}

		auto widget = actorWidget->second;

		if (widget->HasParent())
			widget->GetParent()->UnconsiderWidget(*widget);

		m_sceneRoot->ConsiderWidget(*widget);
	}
}

void OvEditor::Panels::Hierarchy::DeleteActorByInstance(OvCore::ECS::Actor& p_actor)
{
	if (auto result = m_widgetActorLink.find(&p_actor); result != m_widgetActorLink.end())
	{
		if (result->second)
		{
			result->second->Destroy();
		}

		m_widgetActorLink.erase(result);
	}
}

void OvEditor::Panels::Hierarchy::AddActorByInstance(OvCore::ECS::Actor & p_actor)
{
	auto& textSelectable = m_sceneRoot->CreateWidget<OvUI::Widgets::Layout::TreeNode>(p_actor.GetName(), true);
	textSelectable.leaf = true;
	textSelectable.AddPlugin<HierarchyActorContextualMenu>(p_actor, textSelectable);
	textSelectable.AddPlugin<OvUI::Plugins::DDSource<std::pair<OvCore::ECS::Actor*, OvUI::Widgets::Layout::TreeNode*>>>("Actor", "Attach to...", std::make_pair(&p_actor, &textSelectable));
	textSelectable.AddPlugin<OvUI::Plugins::DDTarget<std::pair<OvCore::ECS::Actor*, OvUI::Widgets::Layout::TreeNode*>>>("Actor").DataReceivedEvent += [&p_actor, &textSelectable](std::pair<OvCore::ECS::Actor*, OvUI::Widgets::Layout::TreeNode*> p_element)
	{
		if (p_element.second->HasParent())
			p_element.second->GetParent()->UnconsiderWidget(*p_element.second);

		textSelectable.ConsiderWidget(*p_element.second);

		p_element.first->SetParent(p_actor);
	};
	auto& dispatcher = textSelectable.AddPlugin<OvUI::Plugins::DataDispatcher<std::string>>();

	OvCore::ECS::Actor* targetPtr = &p_actor;
	dispatcher.RegisterGatherer([targetPtr] { return targetPtr->GetName(); });

	m_widgetActorLink[targetPtr] = &textSelectable;

	textSelectable.ClickedEvent += EDITOR_BIND(SelectActor, std::ref(p_actor));
}