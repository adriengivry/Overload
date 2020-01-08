/**
* @project: Overload
* @author: Overload Tech.
* @licence: MIT
*/

#include <OvTools/Filesystem/tinyxml2.h>
#include <OvWindowing/Dialogs/MessageBox.h>

#include "OvCore/SceneSystem/SceneManager.h"
#include "OvCore/ECS/Components/CDirectionalLight.h"
#include "OvCore/ECS/Components/CAmbientSphereLight.h"
#include "OvCore/ECS/Components/CCamera.h"

OvCore::SceneSystem::SceneManager::SceneManager(const std::string& p_sceneRootFolder) : m_sceneRootFolder(p_sceneRootFolder)
{
	LoadEmptyScene();
}

OvCore::SceneSystem::SceneManager::~SceneManager()
{
	UnloadCurrentScene();
}

void OvCore::SceneSystem::SceneManager::Update()
{
	if (m_delayedLoadCall)
	{
		m_delayedLoadCall();
		m_delayedLoadCall = 0;
	}
}

void OvCore::SceneSystem::SceneManager::LoadAndPlayDelayed(const std::string& p_path, bool p_absolute)
{
	m_delayedLoadCall = [this, p_path, p_absolute]
	{
		std::string previousSourcePath = GetCurrentSceneSourcePath();
		LoadScene(p_path, p_absolute);
		StoreCurrentSceneSourcePath(previousSourcePath);
		GetCurrentScene()->Play();
	};
}

void OvCore::SceneSystem::SceneManager::LoadEmptyScene()
{
	UnloadCurrentScene();

	m_currentScene = new Scene();

	SceneLoadEvent.Invoke();
}

void OvCore::SceneSystem::SceneManager::LoadEmptyLightedScene()
{
	UnloadCurrentScene();

	m_currentScene = new Scene();

	SceneLoadEvent.Invoke();

	auto& directionalLight = m_currentScene->CreateActor("Directional Light");
	directionalLight.AddComponent<ECS::Components::CDirectionalLight>().SetIntensity(0.75f);
	directionalLight.transform.SetLocalPosition({ 0.0f, 10.0f, 0.0f });
	directionalLight.transform.SetLocalRotation(OvMaths::FQuaternion({ 120.0f, -40.0f, 0.0f }));

	auto& ambientLight = m_currentScene->CreateActor("Ambient Light");
	ambientLight.AddComponent<ECS::Components::CAmbientSphereLight>().SetRadius(10000.0f);

	auto& camera = m_currentScene->CreateActor("Main Camera");
	camera.AddComponent<ECS::Components::CCamera>();
	camera.transform.SetLocalPosition({ 0.0f, 3.0f, 8.0f });
	camera.transform.SetLocalRotation(OvMaths::FQuaternion({ 20.0f, 180.0f, 0.0f }));
}

bool OvCore::SceneSystem::SceneManager::LoadScene(const std::string& p_path, bool p_absolute)
{
	std::string completePath = (p_absolute ? "" : m_sceneRootFolder) + p_path;

	tinyxml2::XMLDocument doc;
	doc.LoadFile(completePath.c_str());

	if (LoadSceneFromMemory(doc))
	{
		StoreCurrentSceneSourcePath(completePath);
		return true;
	}

	return false;
}

bool OvCore::SceneSystem::SceneManager::LoadSceneFromMemory(tinyxml2::XMLDocument& p_doc)
{
	if (!p_doc.Error())
	{
		tinyxml2::XMLNode* root = p_doc.FirstChild();
		if (root)
		{
			tinyxml2::XMLNode* sceneNode = root->FirstChildElement("scene");
			if (sceneNode)
			{
				LoadEmptyScene();
				m_currentScene->OnDeserialize(p_doc, sceneNode);
				return true;
			}
		}
	}

	OvWindowing::Dialogs::MessageBox message("Scene loading failed", "The scene you are trying to load was not found or corrupted", OvWindowing::Dialogs::MessageBox::EMessageType::ERROR, OvWindowing::Dialogs::MessageBox::EButtonLayout::OK, true);
	return false;
}

void OvCore::SceneSystem::SceneManager::UnloadCurrentScene()
{
	if (m_currentScene)
	{
		delete m_currentScene;
		m_currentScene = nullptr;
		SceneUnloadEvent.Invoke();
	}

	ForgetCurrentSceneSourcePath();
}

bool OvCore::SceneSystem::SceneManager::HasCurrentScene() const
{
	return m_currentScene;
}

OvCore::SceneSystem::Scene* OvCore::SceneSystem::SceneManager::GetCurrentScene()
{
	return m_currentScene;
}

std::string OvCore::SceneSystem::SceneManager::GetCurrentSceneSourcePath() const
{
	return m_currentSceneSourcePath;
}

bool OvCore::SceneSystem::SceneManager::IsCurrentSceneLoadedFromDisk() const
{
	return m_currentSceneLoadedFromPath;
}

void OvCore::SceneSystem::SceneManager::StoreCurrentSceneSourcePath(const std::string& p_path)
{
	m_currentSceneSourcePath = p_path;
	m_currentSceneLoadedFromPath = true;
	CurrentSceneSourcePathChangedEvent.Invoke(m_currentSceneSourcePath);
}

void OvCore::SceneSystem::SceneManager::ForgetCurrentSceneSourcePath()
{
	m_currentSceneSourcePath = "";
	m_currentSceneLoadedFromPath = false;
	CurrentSceneSourcePathChangedEvent.Invoke(m_currentSceneSourcePath);
}
