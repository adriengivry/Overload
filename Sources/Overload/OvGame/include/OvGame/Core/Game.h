/**
* @project: Overload
* @author: Overload Tech.
* @licence: MIT
*/

#pragma once

#include <OvUI/Modules/Canvas.h>
#include <OvCore/ECS/Components/CCamera.h>

#include "OvGame/Core/Context.h"
#include "OvGame/Core/GameRenderer.h"

#include "OvGame/Utils/FPSCounter.h"

#ifdef _DEBUG
#include "OvGame/Debug/DriverInfo.h"
#include "OvGame/Debug/GameProfiler.h"
#include "OvGame/Debug/FrameInfo.h"
#endif

namespace OvGame::Core
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
		float m_elapsed = 0.0f;

		OvGame::Core::Context& m_context;
		OvUI::Modules::Canvas m_canvas;

		OvGame::Core::GameRenderer m_gameRenderer;

		/* Debug elements */
		OvGame::Utils::FPSCounter	m_fpsCounter;

		#ifdef _DEBUG
		OvGame::Debug::DriverInfo	m_driverInfo;
		OvGame::Debug::GameProfiler m_gameProfiler;
		OvGame::Debug::FrameInfo	m_frameInfo;
		#endif

		#ifdef _DEBUG
		bool m_showDebugInformation = true;
		#else
		bool m_showDebugInformation = false;
		#endif
	};
}