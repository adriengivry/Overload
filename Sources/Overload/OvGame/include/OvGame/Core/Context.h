/**
* @project: Overload
* @author: Overload Tech.
* @licence: MIT
*/

#pragma once

#include <OvRendering/Buffers/UniformBuffer.h>
#include <OvRendering/Buffers/ShaderStorageBuffer.h>

#include <OvPhysics/Core/PhysicsEngine.h>

#include <OvWindowing/Context/Device.h>
#include <OvWindowing/Inputs/InputManager.h>
#include <OvWindowing/Window.h>

#include <OvUI/Core/UIManager.h>

#include <OvCore/ECS/Renderer.h>
#include <OvCore/ResourceManagement/ModelManager.h>
#include <OvCore/ResourceManagement/TextureManager.h>
#include <OvCore/ResourceManagement/ShaderManager.h>
#include <OvCore/ResourceManagement/MaterialManager.h>
#include <OvCore/ResourceManagement/SoundManager.h>
#include <OvCore/SceneSystem/SceneManager.h>
#include <OvCore/Scripting/ScriptInterpreter.h>

#include <OvAudio/Core/AudioEngine.h>
#include <OvAudio/Core/AudioPlayer.h>

#include <OvTools/Filesystem/IniFile.h>

namespace OvGame::Core
{
	/**
	* The Context handle the engine features setup
	*/
	class Context
	{
	public:
		/**
		* Constructor
		*/
		Context();

		/**
		* Destructor
		*/
		~Context();

	public:
		const std::string engineAssetsPath;
		const std::string projectAssetsPath;
		const std::string projectScriptsPath;

		std::unique_ptr<OvWindowing::Context::Device>				device;
		std::unique_ptr<OvWindowing::Window>						window;
		std::unique_ptr<OvWindowing::Inputs::InputManager>			inputManager;
		std::unique_ptr<OvRendering::Context::Driver>				driver;
		std::unique_ptr<OvCore::ECS::Renderer>						renderer;
		std::unique_ptr<OvUI::Core::UIManager>						uiManager;
		std::unique_ptr<OvPhysics::Core::PhysicsEngine>				physicsEngine;
		std::unique_ptr<OvAudio::Core::AudioEngine>					audioEngine;
		std::unique_ptr<OvAudio::Core::AudioPlayer>					audioPlayer;
		std::unique_ptr<OvCore::Scripting::ScriptInterpreter>		scriptInterpreter;
		std::unique_ptr<OvRendering::Buffers::UniformBuffer>		engineUBO;
		std::unique_ptr<OvRendering::Buffers::ShaderStorageBuffer>	lightSSBO;

		OvCore::SceneSystem::SceneManager sceneManager;

		OvCore::ResourceManagement::ModelManager	modelManager;
		OvCore::ResourceManagement::TextureManager	textureManager;
		OvCore::ResourceManagement::ShaderManager	shaderManager;
		OvCore::ResourceManagement::MaterialManager	materialManager;
		OvCore::ResourceManagement::SoundManager	soundManager;
		
		OvTools::Filesystem::IniFile projectSettings;
	};
}