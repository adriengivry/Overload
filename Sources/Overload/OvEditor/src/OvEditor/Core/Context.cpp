/**
* @project: Overload
* @author: Overload Tech.
* @licence: MIT
*/

#include <filesystem>

#include <OvRendering/Entities/Light.h>
#include <OvCore/Global/ServiceLocator.h>

#include "OvEditor/Core/Context.h"

using namespace OvCore::Global;
using namespace OvCore::ResourceManagement;

OvEditor::Core::Context::Context(const std::string& p_projectPath, const std::string& p_projectName) :
	projectPath(p_projectPath),
	projectName(p_projectName),
	projectFilePath(p_projectPath + p_projectName + ".ovproject"),
	engineAssetsPath(std::filesystem::canonical("Data\\Engine").string() + "\\"),
	projectAssetsPath(p_projectPath + "Assets\\"),
	projectScriptsPath(p_projectPath + "Scripts\\"),
	editorAssetsPath("Data\\Editor\\"),
	sceneManager(projectAssetsPath),
	projectSettings(projectFilePath)
{
	if (!IsProjectSettingsIntegrityVerified())
	{
		ResetProjectSettings();
		projectSettings.Rewrite();
	}

	ModelManager::ProvideAssetPaths(projectAssetsPath, engineAssetsPath);
	TextureManager::ProvideAssetPaths(projectAssetsPath, engineAssetsPath);
	ShaderManager::ProvideAssetPaths(projectAssetsPath, engineAssetsPath);
	MaterialManager::ProvideAssetPaths(projectAssetsPath, engineAssetsPath);
	SoundManager::ProvideAssetPaths(projectAssetsPath, engineAssetsPath);

	/* Settings */
	OvWindowing::Settings::DeviceSettings deviceSettings;
	deviceSettings.contextMajorVersion = 4;
	deviceSettings.contextMinorVersion = 3;
	windowSettings.title = "Overload Editor";
	windowSettings.width = 1280;
	windowSettings.height = 720;
	windowSettings.maximized = true;

	/* Window creation */
	device = std::make_unique<OvWindowing::Context::Device>(deviceSettings);
	window = std::make_unique<OvWindowing::Window>(*device, windowSettings);
	std::vector<uint64_t> iconRaw = { 0,0,144115188614240000,7500771567664627712,7860776967494637312,0,0,0,0,7212820467466371072,11247766461832697600,14274185407633888512,12905091124788992000,5626708973701824512,514575842263176960,0,0,6564302121125019648,18381468271671515136,18381468271654737920,18237353083595659264,18165295488836311040,6708138037527189504,0,4186681893338480640,7932834557741046016,17876782538917681152,11319824055216379904,15210934132358518784,18381468271520454400,1085667680982603520,0,18093237891929479168,18309410677600032768,11391881649237530624,7932834561381570304,17300321784231761408,15210934132375296000,8293405106311272448,2961143145139082752,16507969723533236736,17516777143216379904,10671305705855129600,7356091234422036224,16580027318695106560,2240567205413984000,18381468271470188544,10959253511276599296,4330520004484136960,10815138323200743424,11607771853338181632,8364614976649238272,17444719546862998784,2669156352,18381468269893064448,6419342512197474304,11103650170688640000,6492244531366860800,14346241902646925312,13841557270159628032,7428148827772098304,3464698581331941120,18381468268953606144,1645680384,18381468271554008832,7140201027266418688,5987558797656659712,17588834734687262208,7284033640602212096,14273902834169157632,18381468269087692288,6852253225049397248,17732667349600245504,16291515470083266560,10022503688432981760,11968059825861367552,9733991836700645376,14850363587428816640,18381468271168132864,16147400282007410688,656430432014827520,18381468270950094848,15715054717226194944,72057596690306560,11823944635485519872,15859169905251653376,17084149004500473856,8581352906816952064,2527949855582584832,18381468271419856896,8581352907253225472,252776704,1376441223417430016,14994761349590357760,10527190521537370112,0,9806614576878321664,18381468271671515136,17156206598538401792,6059619689256392448,10166619973990488064,18381468271403079424,17444719549178451968,420746240,870625192710242304,4906133035823863552,18381468269289150464,18381468271671515136,18381468271671515136,9950729769032620032,14778305994951169792,269422336,0,0,18381468268785833984,8941923452686178304,18381468270950094848,3440842496,1233456333565402880,0,0,0,11823944636091210240,2383877888,16724143605745719296,2316834816,0,0 };
	window->SetIconFromMemory(reinterpret_cast<uint8_t*>(iconRaw.data()), 16, 16);
	inputManager = std::make_unique<OvWindowing::Inputs::InputManager>(*window);
	window->MakeCurrentContext();

	device->SetVsync(true);

	/* Graphics context creation */
	driver = std::make_unique<OvRendering::Context::Driver>(OvRendering::Settings::DriverSettings{ true });
	renderer = std::make_unique<OvCore::ECS::Renderer>(*driver);
	renderer->SetCapability(OvRendering::Settings::ERenderingCapability::MULTISAMPLE, true);
	shapeDrawer = std::make_unique<OvRendering::Core::ShapeDrawer>(*renderer);

	std::filesystem::create_directories(std::string(getenv("APPDATA")) + "\\OverloadTech\\OvEditor\\");

	uiManager = std::make_unique<OvUI::Core::UIManager>(window->GetGlfwWindow(), OvUI::Styling::EStyle::ALTERNATIVE_DARK);
	uiManager->LoadFont("Ruda_Big", editorAssetsPath + "\\Fonts\\Ruda-Bold.ttf", 16);
	uiManager->LoadFont("Ruda_Small", editorAssetsPath + "\\Fonts\\Ruda-Bold.ttf", 12);
	uiManager->LoadFont("Ruda_Medium", editorAssetsPath + "\\Fonts\\Ruda-Bold.ttf", 14);
	uiManager->UseFont("Ruda_Medium");
	uiManager->SetEditorLayoutSaveFilename(std::string(getenv("APPDATA")) + "\\OverloadTech\\OvEditor\\layout.ini");
	uiManager->SetEditorLayoutAutosaveFrequency(60.0f);
	uiManager->EnableEditorLayoutSave(true);
	uiManager->EnableDocking(true);

	if (!std::filesystem::exists(std::string(getenv("APPDATA")) + "\\OverloadTech\\OvEditor\\layout.ini"))
		uiManager->ResetLayout("Config\\layout.ini");

	/* Audio */
	audioEngine = std::make_unique<OvAudio::Core::AudioEngine>(projectAssetsPath);
	audioPlayer = std::make_unique<OvAudio::Core::AudioPlayer>(*audioEngine);

	/* Editor resources */
	editorResources = std::make_unique<OvEditor::Core::EditorResources>(editorAssetsPath);

	/* Physics engine */
	physicsEngine = std::make_unique<OvPhysics::Core::PhysicsEngine>(OvPhysics::Settings::PhysicsSettings{ {0.0f, -9.81f, 0.0f } });

	/* Service Locator providing */
	ServiceLocator::Provide<OvPhysics::Core::PhysicsEngine>(*physicsEngine);
	ServiceLocator::Provide<ModelManager>(modelManager);
	ServiceLocator::Provide<TextureManager>(textureManager);
	ServiceLocator::Provide<ShaderManager>(shaderManager);
	ServiceLocator::Provide<MaterialManager>(materialManager);
	ServiceLocator::Provide<SoundManager>(soundManager);
	ServiceLocator::Provide<OvWindowing::Inputs::InputManager>(*inputManager);
	ServiceLocator::Provide<OvWindowing::Window>(*window);
	ServiceLocator::Provide<OvCore::SceneSystem::SceneManager>(sceneManager);
	ServiceLocator::Provide<OvAudio::Core::AudioEngine>(*audioEngine);
	ServiceLocator::Provide<OvAudio::Core::AudioPlayer>(*audioPlayer);

	/* Scripting */
	scriptInterpreter = std::make_unique<OvCore::Scripting::ScriptInterpreter>(projectScriptsPath);

	engineUBO = std::make_unique<OvRendering::Buffers::UniformBuffer>
	(
		/* UBO Data Layout */
		sizeof(OvMaths::FMatrix4) +
		sizeof(OvMaths::FMatrix4) +
		sizeof(OvMaths::FMatrix4) +
		sizeof(OvMaths::FVector3) +
		sizeof(float) +
		sizeof(OvMaths::FMatrix4),
		0, 0,
		OvRendering::Buffers::EAccessSpecifier::STREAM_DRAW
	);

	lightSSBO			= std::make_unique<OvRendering::Buffers::ShaderStorageBuffer>(OvRendering::Buffers::EAccessSpecifier::STREAM_DRAW);
	simulatedLightSSBO	= std::make_unique<OvRendering::Buffers::ShaderStorageBuffer>(OvRendering::Buffers::EAccessSpecifier::STREAM_DRAW); // Used in Asset View

	std::vector<OvMaths::FMatrix4> simulatedLights;

	OvMaths::FTransform simulatedLightTransform;
	simulatedLightTransform.SetLocalRotation(OvMaths::FQuaternion({ 45.f, 180.f, 10.f }));

	OvRendering::Entities::Light simulatedDirectionalLight(simulatedLightTransform, OvRendering::Entities::Light::Type::DIRECTIONAL);
	simulatedDirectionalLight.color = { 1.f, 1.f, 1.f };
	simulatedDirectionalLight.intensity = 1.f;

	OvRendering::Entities::Light simulatedAmbientLight(simulatedLightTransform, OvRendering::Entities::Light::Type::AMBIENT_SPHERE);
	simulatedAmbientLight.color = { 0.07f, 0.07f, 0.07f };
	simulatedAmbientLight.intensity = 1.f;
	simulatedAmbientLight.constant = 1000.0f;

	simulatedLights.push_back(simulatedDirectionalLight.GenerateMatrix());
	simulatedLights.push_back(simulatedAmbientLight.GenerateMatrix());

	simulatedLightSSBO->SendBlocks<OvMaths::FMatrix4>(simulatedLights.data(), simulatedLights.size() * sizeof(OvMaths::FMatrix4));

	ApplyProjectSettings();
}

OvEditor::Core::Context::~Context()
{
	modelManager.UnloadResources();
	textureManager.UnloadResources();
	shaderManager.UnloadResources();
	materialManager.UnloadResources();
	soundManager.UnloadResources();
}

void OvEditor::Core::Context::ResetProjectSettings()
{
	projectSettings.RemoveAll();
	projectSettings.Add<float>("gravity", -9.81f);
	projectSettings.Add<int>("x_resolution", 1280);
	projectSettings.Add<int>("y_resolution", 720);
	projectSettings.Add<bool>("fullscreen", false);
	projectSettings.Add<std::string>("executable_name", "Game");
	projectSettings.Add<std::string>("start_scene", "Scene.ovscene");
	projectSettings.Add<bool>("vsync", true);
	projectSettings.Add<bool>("multisampling", true);
	projectSettings.Add<int>("samples", 4);
	projectSettings.Add<int>("opengl_major", 4);
	projectSettings.Add<int>("opengl_minor", 3);
	projectSettings.Add<bool>("dev_build", true);
}

bool OvEditor::Core::Context::IsProjectSettingsIntegrityVerified()
{
	return
		projectSettings.IsKeyExisting("gravity") &&
		projectSettings.IsKeyExisting("x_resolution") &&
		projectSettings.IsKeyExisting("y_resolution") &&
		projectSettings.IsKeyExisting("fullscreen") &&
		projectSettings.IsKeyExisting("executable_name") &&
		projectSettings.IsKeyExisting("start_scene") &&
		projectSettings.IsKeyExisting("vsync") &&
		projectSettings.IsKeyExisting("multisampling") &&
		projectSettings.IsKeyExisting("samples") &&
		projectSettings.IsKeyExisting("opengl_major") &&
		projectSettings.IsKeyExisting("opengl_minor") &&
		projectSettings.IsKeyExisting("dev_build");
}

void OvEditor::Core::Context::ApplyProjectSettings()
{
	physicsEngine->SetGravity({ 0.0f, projectSettings.Get<float>("gravity"), 0.0f });
}
