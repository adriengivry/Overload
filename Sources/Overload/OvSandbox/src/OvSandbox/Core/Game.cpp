/**
* @project: Overload
* @author: Overload Tech.
* @licence: MIT
*/

#include "OvSandbox/Core/Game.h"

#include <OvDebug/Logger.h>
#include <OvUI/Widgets/Texts/Text.h>

#include <OvAnalytics/Profiling/ProfilerSpy.h>

#ifdef _DEBUG
#include <OvRendering/Features/FrameInfoRenderFeature.h>
#include <OvRendering/Features/DebugShapeRenderFeature.h>
#endif

#include <OvCore/ECS/Components/CCamera.h>
#include <OvCore/ECS/Components/CMaterialRenderer.h>
#include <OvCore/ECS/Components/CModelRenderer.h>

OvSandbox::Core::Game::Game(Context & p_context) :
	m_context(p_context),
	m_sceneRenderer(*p_context.driver),
	m_fpsCounter(*p_context.window)
	#ifdef _DEBUG
	,
	m_driverInfo(*m_context.driver, *m_context.window),
	m_gameProfiler(*p_context.window, 0.25f),
	m_frameInfo(*m_context.window)
	#endif
{
	m_context.uiManager->SetCanvas(m_canvas);
	m_canvas.AddPanel(m_fpsCounter);
	#ifdef _DEBUG
	m_canvas.AddPanel(m_driverInfo);
	m_canvas.AddPanel(m_gameProfiler);
	m_canvas.AddPanel(m_frameInfo);
	m_sceneRenderer.AddFeature<OvRendering::Features::FrameInfoRenderFeature>();
	m_sceneRenderer.AddFeature<OvRendering::Features::DebugShapeRenderFeature>();
	#endif

	m_context.sceneManager.LoadEmptyLightedScene();

	using namespace OvCore::ECS::Components;

	auto& scene = *m_context.sceneManager.GetCurrentScene();

	auto& object = scene.CreateActor("Object");
	object.transform.SetWorldPosition({ 0.0f, 0.0f, 0.0f });
	auto& modelRenderer = object.AddComponent<CModelRenderer>();
	auto cube = m_context.modelManager.LoadResource(":Models\\Cube.fbx");
	modelRenderer.SetModel(cube);
	auto& materialRenderer = object.AddComponent<CMaterialRenderer>();
	auto material = m_context.materialManager.LoadResource(":Materials\\Default.ovmat");
	materialRenderer.SetMaterialAtIndex(0, *material);

	scene.Play();
}

OvSandbox::Core::Game::~Game()
{
	m_context.sceneManager.UnloadCurrentScene();
}

void OvSandbox::Core::Game::PreUpdate()
{
	#ifdef _DEBUG
	PROFILER_SPY("Pre-Update");
	#endif
	m_context.device->PollEvents();
}

void RenderCurrentScene(
	OvCore::Rendering::SceneRenderer& p_renderer,
	const OvSandbox::Core::Context& p_context
)
{
#ifdef _DEBUG
	PROFILER_SPY("Render Current Scene");
#endif

	if (auto currentScene = p_context.sceneManager.GetCurrentScene())
	{
		if (auto camera = currentScene->FindMainCamera())
		{
			auto [windowWidth, windowHeight] = p_context.window->GetSize();

			p_renderer.AddDescriptor<OvCore::Rendering::SceneRenderer::SceneDescriptor>({
				*currentScene,
			});

			OvRendering::Data::FrameDescriptor frameDescriptor;
			frameDescriptor.renderWidth = windowWidth;
			frameDescriptor.renderHeight = windowHeight;
			frameDescriptor.camera = camera->GetCamera();

			p_renderer.BeginFrame(frameDescriptor);
			p_renderer.Draw();
			auto pso = p_renderer.CreatePipelineState();
			p_renderer.GetFeature<OvRendering::Features::DebugShapeRenderFeature>().DrawLine(
				pso, {0.0f, -100.0f, 0.0f}, {0.0f, 100.0f, 0.0f}, {1.0f, 0.0f, 0.0f}, 10.0f);
			p_renderer.GetFeature<OvRendering::Features::DebugShapeRenderFeature>().DrawLine(
				pso, { -100.0f, 0.0f, 0.0f }, { 100.0f, 0.0f, 0.0f }, { 0.0f, 1.0f, 0.0f }, 10.0f);
			p_renderer.GetFeature<OvRendering::Features::DebugShapeRenderFeature>().DrawLine(
				pso, { 0.0f, 0.0f, -100.0f }, { 0.0f, 0.0f, 100.0f }, { 0.0f, 0.0f, 1.0f }, 10.0f);
			p_renderer.EndFrame();
		}
	}
}

void OvSandbox::Core::Game::Update(float p_deltaTime)
{
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

		if (auto currentScene = m_context.sceneManager.GetCurrentScene())
		{
			#ifdef _DEBUG
			PROFILER_SPY("Render Scene");
			#endif

			auto [windowWidth, windowHeight] = m_context.window->GetSize();

			RenderCurrentScene(m_sceneRenderer, m_context);
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
		auto& frameInfoRenderFeature = m_sceneRenderer.GetFeature<OvRendering::Features::FrameInfoRenderFeature>();
		auto& frameInfo = frameInfoRenderFeature.GetFrameInfo();
		m_frameInfo.Update(frameInfo);
		#endif
		m_context.uiManager->Render();
	}
}

void OvSandbox::Core::Game::PostUpdate()
{
	#ifdef _DEBUG
	PROFILER_SPY("Post-Update");
	#endif
	m_context.window->SwapBuffers();
	m_context.inputManager->ClearEvents();
}