/**
* @project: Overload
* @author: Overload Tech.
* @licence: MIT
*/

#pragma once

#include <OvUI/Modules/Canvas.h>
#include <OvCore/ECS/Components/CCamera.h>
#include <OvCore/Rendering/SceneRenderer.h>

#include "OvSandbox/Core/Context.h"
#include "OvSandbox/Utils/FPSCounter.h"

#ifdef _DEBUG
#include "OvSandbox/Debug/DriverInfo.h"
#include "OvSandbox/Debug/GameProfiler.h"
#include "OvSandbox/Debug/FrameInfo.h"
#endif

namespace OvSandbox::Core
{
	/**
	* Handle the game logic
	*/
	class Game
	{
	public:
		/**
		* Create the game
		* @param p_context
		*/
		Game(Context& p_context);

		/**
		* Destroy the game
		*/
		~Game();

		/**
		* Pre-update of the game logic
		*/
		void PreUpdate();

		/**
		* Update the game logic
		* @param p_deltaTime
		*/
		void Update(float p_deltaTime);

		/**
		* Post-update of the game logic
		*/
		void PostUpdate();

	private:
		OvSandbox::Core::Context& m_context;
		OvUI::Modules::Canvas m_canvas;

		OvCore::Rendering::SceneRenderer m_sceneRenderer;

		/* Debug elements */
		OvSandbox::Utils::FPSCounter	m_fpsCounter;

		#ifdef _DEBUG
		OvSandbox::Debug::DriverInfo	m_driverInfo;
		OvSandbox::Debug::GameProfiler m_gameProfiler;
		OvSandbox::Debug::FrameInfo	m_frameInfo;
		#endif

		#ifdef _DEBUG
		bool m_showDebugInformation = true;
		#else
		bool m_showDebugInformation = false;
		#endif
	};
}