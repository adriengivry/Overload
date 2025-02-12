/**
* @project: Overload
* @author: Overload Tech.
* @licence: MIT
*/

#include <OvAnalytics/Profiling/ProfilerSpy.h>
#include <OvPhysics/Core/PhysicsEngine.h>

#include "OvEditor/Core/Editor.h"
#include "OvEditor/Panels/MenuBar.h"
#include "OvEditor/Panels/AssetBrowser.h"
#include "OvEditor/Panels/HardwareInfo.h"
#include "OvEditor/Panels/Profiler.h"
#include "OvEditor/Panels/FrameInfo.h"
#include "OvEditor/Panels/Console.h"
#include "OvEditor/Panels/Inspector.h"
#include "OvEditor/Panels/Hierarchy.h"
#include "OvEditor/Panels/SceneView.h"
#include "OvEditor/Panels/GameView.h"
#include "OvEditor/Panels/AssetView.h"
#include "OvEditor/Panels/Toolbar.h"
#include "OvEditor/Panels/MaterialEditor.h"
#include "OvEditor/Panels/ProjectSettings.h"
#include "OvEditor/Panels/AssetProperties.h"

using namespace OvCore::ResourceManagement;
using namespace OvEditor::Panels;
using namespace OvRendering::Resources::Loaders;
using namespace OvRendering::Resources::Parsers;

OvEditor::Core::Editor::Editor(Context& p_context) : 
	m_context(p_context),
	m_panelsManager(m_canvas),
	m_editorActions(m_context, m_panelsManager)
{
	SetupUI();

	m_context.sceneManager.LoadEmptyLightedScene();
}

OvEditor::Core::Editor::~Editor()
{
	m_context.sceneManager.UnloadCurrentScene();
}

void OvEditor::Core::Editor::SetupUI()
{
	OvUI::Settings::PanelWindowSettings settings;
	settings.closable = true;
	settings.collapsable = true;
	settings.dockable = true;

	m_panelsManager.CreatePanel<Panels::MenuBar>("Menu Bar");
	m_panelsManager.CreatePanel<Panels::AssetBrowser>("Asset Browser", true, settings, m_context.engineAssetsPath, m_context.projectAssetsPath, m_context.projectScriptsPath);
	m_panelsManager.CreatePanel<Panels::HardwareInfo>("Hardware Info", false, settings, 0.2f, 50);
	m_panelsManager.CreatePanel<Panels::Profiler>("Profiler", true, settings, 0.25f);
	m_panelsManager.CreatePanel<Panels::FrameInfo>("Frame Info", true, settings);
	m_panelsManager.CreatePanel<Panels::Console>("Console", true, settings);
	m_panelsManager.CreatePanel<Panels::AssetView>("Asset View", false, settings);
	m_panelsManager.CreatePanel<Panels::Hierarchy>("Hierarchy", true, settings);
	m_panelsManager.CreatePanel<Panels::Inspector>("Inspector", true, settings);
	m_panelsManager.CreatePanel<Panels::SceneView>("Scene View", true, settings);
	m_panelsManager.CreatePanel<Panels::GameView>("Game View", true, settings);
	m_panelsManager.CreatePanel<Panels::Toolbar>("Toolbar", true, settings);
	m_panelsManager.CreatePanel<Panels::MaterialEditor>("Material Editor", false, settings);
	m_panelsManager.CreatePanel<Panels::ProjectSettings>("Project Settings", false, settings);
	m_panelsManager.CreatePanel<Panels::AssetProperties>("Asset Properties", false, settings);

	// Needs to be called after all panels got created, because some settings in this menu depend on other panels
	m_panelsManager.GetPanelAs<Panels::MenuBar>("Menu Bar").InitializeSettingsMenu();

	m_canvas.MakeDockspace(true);
	m_context.uiManager->SetCanvas(m_canvas);
}

void OvEditor::Core::Editor::PreUpdate()
{
	PROFILER_SPY("Editor Pre-Update");
	m_context.device->PollEvents();
}

void OvEditor::Core::Editor::Update(float p_deltaTime)
{
	for (size_t i = 0; i < 50; i++)
	{
		OVLOG("Coucou");
	}

	HandleGlobalShortcuts();
	UpdateCurrentEditorMode(p_deltaTime);
	RenderViews(p_deltaTime);
	UpdateEditorPanels(p_deltaTime);
	RenderEditorUI(p_deltaTime);
	m_editorActions.ExecuteDelayedActions();
}

void OvEditor::Core::Editor::HandleGlobalShortcuts()
{
	// If the [Del] key is pressed while an actor is selected and the Scene View or Hierarchy is focused
	if (m_context.inputManager->IsKeyPressed(OvWindowing::Inputs::EKey::KEY_DELETE) && EDITOR_EXEC(IsAnyActorSelected()) && (EDITOR_PANEL(SceneView, "Scene View").IsFocused() || EDITOR_PANEL(Hierarchy, "Hierarchy").IsFocused()))
	{
		EDITOR_EXEC(DestroyActor(EDITOR_EXEC(GetSelectedActor())));
	}
}

void OvEditor::Core::Editor::UpdateCurrentEditorMode(float p_deltaTime)
{
	if (auto editorMode = m_editorActions.GetCurrentEditorMode(); editorMode == EditorActions::EEditorMode::PLAY || editorMode == EditorActions::EEditorMode::FRAME_BY_FRAME)
		UpdatePlayMode(p_deltaTime);
	else
		UpdateEditMode(p_deltaTime);

	{
		PROFILER_SPY("Scene garbage collection");
		m_context.sceneManager.GetCurrentScene()->CollectGarbages();
		m_context.sceneManager.Update();
	}
}

void OvEditor::Core::Editor::UpdatePlayMode(float p_deltaTime)
{
	auto currentScene = m_context.sceneManager.GetCurrentScene();
	bool simulationApplied = false;

	{
		PROFILER_SPY("Physics Update");
		simulationApplied = m_context.physicsEngine->Update(p_deltaTime);
	}

	if (simulationApplied)
	{
		PROFILER_SPY("FixedUpdate");
		currentScene->FixedUpdate(p_deltaTime);
	}

	{
		PROFILER_SPY("Update");
		currentScene->Update(p_deltaTime);
	}

	{
		PROFILER_SPY("LateUpdate");
		currentScene->LateUpdate(p_deltaTime);
	}

	{
		PROFILER_SPY("Audio Update");
		m_context.audioEngine->Update();
	}

	ImGui::GetIO().DisableMouseUpdate = m_context.window->GetCursorMode() == OvWindowing::Cursor::ECursorMode::DISABLED;

	if (m_editorActions.GetCurrentEditorMode() == EditorActions::EEditorMode::FRAME_BY_FRAME)
		m_editorActions.PauseGame();

	if (m_context.inputManager->IsKeyPressed(OvWindowing::Inputs::EKey::KEY_ESCAPE))
		m_editorActions.StopPlaying();
}

void OvEditor::Core::Editor::UpdateEditMode(float p_deltaTime)
{
	if (m_context.inputManager->IsKeyPressed(OvWindowing::Inputs::EKey::KEY_F5))
		m_editorActions.StartPlaying();
}

void OvEditor::Core::Editor::UpdateEditorPanels(float p_deltaTime)
{
	auto& menuBar = m_panelsManager.GetPanelAs<OvEditor::Panels::MenuBar>("Menu Bar");
	auto& profiler = m_panelsManager.GetPanelAs<OvEditor::Panels::Profiler>("Profiler");
	auto& frameInfo = m_panelsManager.GetPanelAs<OvEditor::Panels::FrameInfo>("Frame Info");
	auto& hardwareInfo = m_panelsManager.GetPanelAs<OvEditor::Panels::HardwareInfo>("Hardware Info");
	auto& sceneView = m_panelsManager.GetPanelAs<OvEditor::Panels::SceneView>("Scene View");
	auto& gameView = m_panelsManager.GetPanelAs<OvEditor::Panels::GameView>("Game View");
	auto& assetView = m_panelsManager.GetPanelAs<OvEditor::Panels::AssetView>("Asset View");

	menuBar.HandleShortcuts(p_deltaTime);

	if (m_elapsedFrames == 1) // Let the first frame happen and then make the scene view the first seen view
		sceneView.Focus();

	if (frameInfo.IsOpened())
	{
		PROFILER_SPY("Frame Info Update");

		if (sceneView.IsFocused())
		{
			frameInfo.Update(sceneView);
		}
		else if (gameView.IsFocused())
		{
			frameInfo.Update(gameView);
		}
		else if (assetView.IsFocused())
		{
			frameInfo.Update(assetView);
		}
		else
		{
			frameInfo.Update(std::nullopt);
		}
	}

	if (profiler.IsOpened())
	{
		PROFILER_SPY("Profiler Update");
		profiler.Update(p_deltaTime);
	}

	if (hardwareInfo.IsOpened())
	{
		PROFILER_SPY("Hardware Info Update");
		hardwareInfo.Update(p_deltaTime);
	}
}

void OvEditor::Core::Editor::RenderViews(float p_deltaTime)
{
	auto& assetView = m_panelsManager.GetPanelAs<OvEditor::Panels::AssetView>("Asset View");
	auto& sceneView = m_panelsManager.GetPanelAs<OvEditor::Panels::SceneView>("Scene View");
	auto& gameView = m_panelsManager.GetPanelAs<OvEditor::Panels::GameView>("Game View");

	{
		PROFILER_SPY("Editor Views Update");

		assetView.Update(p_deltaTime);
		gameView.Update(p_deltaTime);
		sceneView.Update(p_deltaTime);
	}

	if (assetView.IsOpened() && assetView.IsVisible())
	{
		PROFILER_SPY("Asset View Rendering");
		assetView.Render();
	}

	if (gameView.IsOpened() && gameView.IsVisible())
	{
		PROFILER_SPY("Game View Rendering");
		gameView.Render();
	}

	if (sceneView.IsOpened() && sceneView.IsVisible())
	{
		PROFILER_SPY("Scene View Rendering");
		sceneView.Render();
	}
}

void OvEditor::Core::Editor::RenderEditorUI(float p_deltaTime)
{
	PROFILER_SPY("Editor UI Rendering");

	EDITOR_CONTEXT(uiManager)->Render();
}

void OvEditor::Core::Editor::PostUpdate()
{
	PROFILER_SPY("Editor Post-Update");

	m_context.window->SwapBuffers();
	m_context.inputManager->ClearEvents();
	++m_elapsedFrames;
}