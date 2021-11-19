/**
* @project: Overload
* @author: Overload Tech.
* @licence: MIT
*/

#include "OvGame/Core/Game.h"

#include <OvDebug/Logger.h>
#include <OvUI/Widgets/Texts/Text.h>

#include <OvAnalytics/Profiling/ProfilerSpy.h>

OvGame::Core::Game::Game(Context & p_context) :
	m_context(p_context),
	m_gameRenderer(p_context),
	m_fpsCounter(*p_context.window)
	#ifdef _DEBUG
	,
	m_driverInfo(*m_context.renderer, *m_context.window),
	m_gameProfiler(*p_context.window, 0.25f),
	m_frameInfo(*m_context.renderer, *m_context.window)
	#endif
{
	m_context.uiManager->SetCanvas(m_canvas);
	m_canvas.AddPanel(m_fpsCounter);
	#ifdef _DEBUG
	m_canvas.AddPanel(m_driverInfo);
	m_canvas.AddPanel(m_gameProfiler);
	m_canvas.AddPanel(m_frameInfo);
	#endif

	m_context.sceneManager.LoadScene(m_context.projectSettings.Get<std::string>("start_scene"));
	m_context.sceneManager.GetCurrentScene()->Play();
}

OvGame::Core::Game::~Game()
{
	m_context.sceneManager.UnloadCurrentScene();
}

void OvGame::Core::Game::PreUpdate()
{
	#ifdef _DEBUG
	PROFILER_SPY("Pre-Update");
	#endif
	m_context.device->PollEvents();
}

void OvGame::Core::Game::Update(float p_deltaTime)
{
	m_elapsed += p_deltaTime;

	m_context.engineUBO->SetSubData(m_elapsed, 3 * sizeof(OvMaths::FMatrix4) + sizeof(OvMaths::FVector3));

	m_context.renderer->ClearFrameInfo();

	if (auto currentScene = m_context.sceneManager.GetCurrentScene())
	{
		{
			#ifdef _DEBUG
			PROFILER_SPY("Physics Update");
			#endif

			if (m_context.physicsEngine->Update(p_deltaTime))
				currentScene->FixedUpdate(p_deltaTime);
		}

		{
			#ifdef _DEBUG
			PROFILER_SPY("Scene Update");
			#endif
			currentScene->Update(p_deltaTime);
			currentScene->LateUpdate(p_deltaTime);
		}

		{
			#ifdef _DEBUG
			PROFILER_SPY("Audio Update");
			#endif
			m_context.audioEngine->Update();
		}

		{
			#ifdef _DEBUG
			PROFILER_SPY("Render Scene");
			#endif
			m_gameRenderer.RenderScene();
		}
	}

	m_context.sceneManager.Update();

	if  (m_context.inputManager->IsKeyPressed(OvWindowing::Inputs::EKey::KEY_F12))
		m_showDebugInformation = !m_showDebugInformation;

	#ifdef _DEBUG
	if (m_context.inputManager->IsKeyPressed(OvWindowing::Inputs::EKey::KEY_R))
		OvRendering::Resources::Loaders::ShaderLoader::Recompile(*m_context.shaderManager[":Shaders\\Standard.glsl"], "Data\\Engine\\Shaders\\Standard.glsl");
	#endif

	if (m_showDebugInformation)
	{
		m_fpsCounter.Update(p_deltaTime);
		#ifdef _DEBUG
		m_gameProfiler.Update(p_deltaTime);
		m_frameInfo.Update(p_deltaTime);
		#endif
		m_context.uiManager->Render();
	}
}

void OvGame::Core::Game::PostUpdate()
{
	#ifdef _DEBUG
	PROFILER_SPY("Post-Update");
	#endif
	m_context.window->SwapBuffers();
	m_context.inputManager->ClearEvents();
}