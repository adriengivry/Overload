/**
* @project: Overload
* @author: Overload Tech.
* @licence: MIT
*/

#include <OvAnalytics/Profiling/ProfilerSpy.h>

#include <OvRendering/Data/Frustum.h>
#include <OvRendering/Features/LightingRenderFeature.h>
#include <OvRendering/Resources/Loaders/ShaderLoader.h>

#include "OvCore/Rendering/SceneRenderer.h"
#include "OvCore/Rendering/EngineBufferRenderFeature.h"
#include "OvCore/Rendering/EngineDrawableDescriptor.h"
#include "OvCore/Rendering/ShadowRenderFeature.h"
#include "OvCore/Rendering/ShadowRenderPass.h"
#include "OvCore/ECS/Components/CModelRenderer.h"
#include "OvCore/ECS/Components/CMaterialRenderer.h"
#include "OvCore/Global/ServiceLocator.h"

struct SceneRenderPassDescriptor
{
	OvCore::Rendering::SceneRenderer::AllDrawables drawables;
};

class OpaqueRenderPass : public OvRendering::Core::ARenderPass
{
public:
	OpaqueRenderPass(OvRendering::Core::CompositeRenderer& p_renderer) : OvRendering::Core::ARenderPass(p_renderer) {}

protected:
	virtual void Draw(OvRendering::Data::PipelineState p_pso) override
	{
		auto& sceneContent = m_renderer.GetDescriptor<SceneRenderPassDescriptor>();

		for (const auto& [distance, drawable] : sceneContent.drawables.opaques)
		{
			m_renderer.DrawEntity(p_pso, drawable);
		}
	}
};

class TransparentRenderPass : public OvRendering::Core::ARenderPass
{
public:
	TransparentRenderPass(OvRendering::Core::CompositeRenderer& p_renderer) : OvRendering::Core::ARenderPass(p_renderer) {}

protected:
	virtual void Draw(OvRendering::Data::PipelineState p_pso) override
	{
		auto& sceneContent = m_renderer.GetDescriptor<SceneRenderPassDescriptor>();

		for (const auto& [distance, drawable] : sceneContent.drawables.transparents)
		{
			m_renderer.DrawEntity(p_pso, drawable);
		}
	}
};

class PostProcessRenderPass : public OvRendering::Core::ARenderPass
{
public:
	struct BloomSettings
	{
		float threshold = 0.6f;
		float radius = 3.0f;
		int kernelSize = 5;
		float intensity = 2.0f;
	};

	struct TonemappingSettings
	{
		float exposure = 1.0f;
	};

	void SetupPostProcessMaterial(OvRendering::Data::Material& material)
	{
		material.SetDepthTest(false);
		material.SetDepthWriting(false);
	}

	PostProcessRenderPass(OvRendering::Core::CompositeRenderer& p_renderer) :
		OvRendering::Core::ARenderPass(p_renderer)
	{
		m_unitQuad = CreateUnitQuad();

		m_blitShader = OvRendering::Resources::Loaders::ShaderLoader::Create("Data\\Engine\\Shaders\\Blit.ovfx");
		m_fxaaShader = OvRendering::Resources::Loaders::ShaderLoader::Create("Data\\Engine\\Shaders\\PostProcess\\FXAA.ovfx");
		m_brightnessShader = OvRendering::Resources::Loaders::ShaderLoader::Create("Data\\Engine\\Shaders\\PostProcess\\Brightness.ovfx");
		m_blurShader = OvRendering::Resources::Loaders::ShaderLoader::Create("Data\\Engine\\Shaders\\PostProcess\\Blur.ovfx");
		m_bloomShader = OvRendering::Resources::Loaders::ShaderLoader::Create("Data\\Engine\\Shaders\\PostProcess\\Bloom.ovfx");
		m_tonemappingShader = OvRendering::Resources::Loaders::ShaderLoader::Create("Data\\Engine\\Shaders\\PostProcess\\Tonemapping.ovfx");

		m_blitMaterial.SetShader(m_blitShader);
		m_fxaaMaterial.SetShader(m_fxaaShader);
		m_brightnessMaterial.SetShader(m_brightnessShader);
		m_blurMaterial.SetShader(m_blurShader);
		m_bloomMaterial.SetShader(m_bloomShader);
		m_tonemappingMaterial.SetShader(m_tonemappingShader);

		SetupPostProcessMaterial(m_blitMaterial);
		SetupPostProcessMaterial(m_fxaaMaterial);
		SetupPostProcessMaterial(m_brightnessMaterial);
		SetupPostProcessMaterial(m_blurMaterial);
		SetupPostProcessMaterial(m_bloomMaterial);
		SetupPostProcessMaterial(m_tonemappingMaterial);
	}

	~PostProcessRenderPass()
	{
		OvRendering::Resources::Loaders::ShaderLoader::Destroy(m_blitShader);
	}

protected:
	virtual std::unique_ptr<OvRendering::Resources::Mesh> CreateUnitQuad() const
	{
		const std::vector<OvRendering::Geometry::Vertex> vertices = {
			{ {-1.0f, -1.0f, 0.0f}, {0.0f, 0.0f} }, // Bottom-left
			{ { 1.0f, -1.0f, 0.0f}, {1.0f, 0.0f} }, // Bottom-right
			{ { 1.0f,  1.0f, 0.0f}, {1.0f, 1.0f} }, // Top-right
			{ {-1.0f,  1.0f, 0.0f}, {0.0f, 1.0f} }  // Top-left
		};

		const std::vector<uint32_t> indices = {
			0, 1, 2, // First triangle
			0, 2, 3  // Second triangle
		};

		return std::make_unique<OvRendering::Resources::Mesh>(vertices, indices, 0);
	}

	virtual void Draw(OvRendering::Data::PipelineState p_pso) override
	{
		auto& framebuffer = m_renderer.GetFrameDescriptor().outputBuffer.value();

		BloomSettings bloomSettings;
		bloomSettings.threshold = 0.98f;
		bloomSettings.radius = 2.0f;
		bloomSettings.kernelSize = 4;
		bloomSettings.intensity = 1.0f;

		TonemappingSettings tonemappingSettings;
		tonemappingSettings.exposure = 1.3f;

		Bloom(p_pso, framebuffer, m_tempBuffers[0], bloomSettings);
		Tonemapping(p_pso, m_tempBuffers[0], m_tempBuffers[1], tonemappingSettings);
		FXAA(p_pso, m_tempBuffers[1], framebuffer);
	}

	void Blit(OvRendering::Data::PipelineState p_pso, OvRendering::Buffers::Framebuffer& p_src, OvRendering::Buffers::Framebuffer& p_dst, OvRendering::Data::Material& p_material)
	{
		m_renderer.Blit(p_pso, p_src, p_dst, *m_unitQuad, p_material);
	}

	void Blit(OvRendering::Data::PipelineState p_pso, OvRendering::Buffers::Framebuffer& p_src, OvRendering::Buffers::Framebuffer& p_dst)
	{
		m_blitMaterial.Set("_InputTexture", p_src.GetTexture(), true);
		Blit(p_pso, p_src, p_dst, m_blitMaterial);
	}

	void FXAA(OvRendering::Data::PipelineState p_pso, OvRendering::Buffers::Framebuffer& src, OvRendering::Buffers::Framebuffer& dst)
	{
		m_fxaaMaterial.Set("_InputTexture", src.GetTexture(), true);
		Blit(p_pso, src, dst, m_fxaaMaterial);
	}

	void Tonemapping(
		OvRendering::Data::PipelineState p_pso,
		OvRendering::Buffers::Framebuffer& src,
		OvRendering::Buffers::Framebuffer& dst,
		const TonemappingSettings& settings
	)
	{
		m_tonemappingMaterial.Set("_InputTexture", src.GetTexture(), true);
		m_tonemappingMaterial.Set("_Exposure", settings.exposure, true);
		Blit(p_pso, src, dst, m_tonemappingMaterial);
	}

	void Bloom(
		OvRendering::Data::PipelineState p_pso,
		OvRendering::Buffers::Framebuffer& src,
		OvRendering::Buffers::Framebuffer& dst,
		const BloomSettings& settings
	)
	{
		// Step 1: Extract bright spots from the source
		m_bloomPingPong[1].Resize(src.GetWidth(), src.GetHeight());

		m_brightnessMaterial.Set("_InputTexture", src.GetTexture(), true);
		m_brightnessMaterial.Set("_Threshold", settings.threshold, true);

		Blit(p_pso, src, m_bloomPingPong[0], m_brightnessMaterial);

		// Step 2: Apply gaussian blur on bright spots (horizontal and vertical)
		bool horizontal = true;

		for (int i = 0; i < 10; ++i) { // Perform 10 blur passes (alternating)
			auto& currentSrc = horizontal ? m_bloomPingPong[0] : m_bloomPingPong[1];
			auto& currentDst = horizontal ? m_bloomPingPong[1] : m_bloomPingPong[0];

			m_blurMaterial.Set("_InputTexture", currentSrc.GetTexture(), true);
			m_blurMaterial.Set("_Horizontal", horizontal ? true : false, true);
			m_blurMaterial.Set("_BlurSize", settings.radius, true);
			m_blurMaterial.Set("_KernelSize", settings.kernelSize, true);

			Blit(p_pso, currentSrc, currentDst, m_blurMaterial);

			horizontal = !horizontal;
		}

		// Step 3: Combine bloom with original framebuffer
		m_bloomMaterial.Set("_InputTexture", src.GetTexture(), true);				 // Original scene
		m_bloomMaterial.Set("_BloomTexture", m_bloomPingPong[0].GetTexture(), true); // Bloom texture
		m_bloomMaterial.Set("_BloomIntensity", settings.intensity, true);							 // Intensity

		Blit(p_pso, m_bloomPingPong[0], dst, m_bloomMaterial);
	}

private:
	std::unique_ptr<OvRendering::Resources::Mesh> m_unitQuad;

	std::array<OvRendering::Buffers::Framebuffer, 2> m_tempBuffers;
	std::array<OvRendering::Buffers::Framebuffer, 2> m_bloomPingPong;
	OvRendering::Resources::Shader* m_blitShader;
	OvRendering::Resources::Shader* m_fxaaShader;
	OvRendering::Resources::Shader* m_brightnessShader;
	OvRendering::Resources::Shader* m_blurShader;
	OvRendering::Resources::Shader* m_bloomShader;
	OvRendering::Resources::Shader* m_tonemappingShader;
	OvRendering::Data::Material m_blitMaterial;
	OvRendering::Data::Material m_fxaaMaterial;
	OvRendering::Data::Material m_brightnessMaterial;
	OvRendering::Data::Material m_blurMaterial;
	OvRendering::Data::Material m_bloomMaterial;
	OvRendering::Data::Material m_tonemappingMaterial;
};

OvCore::Rendering::SceneRenderer::SceneRenderer(OvRendering::Context::Driver& p_driver)
	: OvRendering::Core::CompositeRenderer(p_driver)
{
	AddFeature<EngineBufferRenderFeature>();
	AddFeature<OvRendering::Features::LightingRenderFeature>();
	AddFeature<ShadowRenderFeature>();

	AddPass<ShadowRenderPass>("Shadows", OvRendering::Settings::ERenderPassOrder::Shadows);
	AddPass<OpaqueRenderPass>("Opaques", OvRendering::Settings::ERenderPassOrder::Opaque);
	AddPass<TransparentRenderPass>("Transparents", OvRendering::Settings::ERenderPassOrder::Transparent);
	AddPass<PostProcessRenderPass>("Post-Process", OvRendering::Settings::ERenderPassOrder::PostProcessing);
}

OvRendering::Features::LightingRenderFeature::LightSet FindActiveLights(const OvCore::SceneSystem::Scene& p_scene)
{
	OvRendering::Features::LightingRenderFeature::LightSet lights;

	const auto& facs = p_scene.GetFastAccessComponents();

	for (auto light : facs.lights)
	{
		if (light->owner.IsActive())
		{
			lights.push_back(std::ref(light->GetData()));
		}
	}

	return lights;
}

void OvCore::Rendering::SceneRenderer::BeginFrame(const OvRendering::Data::FrameDescriptor& p_frameDescriptor)
{
	OVASSERT(HasDescriptor<SceneDescriptor>(), "Cannot find SceneDescriptor attached to this renderer");

	auto& sceneDescriptor = GetDescriptor<SceneDescriptor>();

	AddDescriptor<OvRendering::Features::LightingRenderFeature::LightingDescriptor>({
		FindActiveLights(sceneDescriptor.scene),
	});

	OvRendering::Core::CompositeRenderer::BeginFrame(p_frameDescriptor);

	AddDescriptor<SceneRenderPassDescriptor>({
		ParseScene()
	});
}

void OvCore::Rendering::SceneRenderer::DrawModelWithSingleMaterial(OvRendering::Data::PipelineState p_pso, OvRendering::Resources::Model& p_model, OvRendering::Data::Material& p_material, const OvMaths::FMatrix4& p_modelMatrix)
{
	auto stateMask = p_material.GenerateStateMask();
	auto userMatrix = OvMaths::FMatrix4::Identity;

	auto engineDrawableDescriptor = EngineDrawableDescriptor{
		p_modelMatrix,
		userMatrix
	};

	for (auto mesh : p_model.GetMeshes())
	{
		OvRendering::Entities::Drawable element;
		element.mesh = *mesh;
		element.material = p_material;
		element.stateMask = stateMask;
		element.AddDescriptor(engineDrawableDescriptor);

		DrawEntity(p_pso, element);
	}
}

OvCore::Rendering::SceneRenderer::AllDrawables OvCore::Rendering::SceneRenderer::ParseScene()
{
	using namespace OvCore::ECS::Components;

	OpaqueDrawables opaques;
	TransparentDrawables transparents;

	auto& camera = m_frameDescriptor.camera.value();

	auto& sceneDescriptor = GetDescriptor<SceneDescriptor>();
	auto& scene = sceneDescriptor.scene;
	auto overrideMaterial = sceneDescriptor.overrideMaterial;
	auto fallbackMaterial = sceneDescriptor.fallbackMaterial;
	std::optional<OvRendering::Data::Frustum> frustum = std::nullopt;

	if (camera.HasFrustumGeometryCulling())
	{
		auto& frustumOverride = sceneDescriptor.frustumOverride;
		frustum = frustumOverride ? frustumOverride : camera.GetFrustum();
	}

	for (CModelRenderer* modelRenderer : scene.GetFastAccessComponents().modelRenderers)
	{
		auto& owner = modelRenderer->owner;

		if (owner.IsActive())
		{
			if (auto model = modelRenderer->GetModel())
			{
				if (auto materialRenderer = modelRenderer->owner.GetComponent<CMaterialRenderer>())
				{
					auto& transform = owner.transform.GetFTransform();

					auto cullingOptions = OvRendering::Settings::ECullingOptions::NONE;

					if (modelRenderer->GetFrustumBehaviour() != CModelRenderer::EFrustumBehaviour::DISABLED)
					{
						cullingOptions |= OvRendering::Settings::ECullingOptions::FRUSTUM_PER_MODEL;
					}

					if (modelRenderer->GetFrustumBehaviour() == CModelRenderer::EFrustumBehaviour::CULL_MESHES)
					{
						cullingOptions |= OvRendering::Settings::ECullingOptions::FRUSTUM_PER_MESH;
					}

					const auto& modelBoundingSphere = modelRenderer->GetFrustumBehaviour() == CModelRenderer::EFrustumBehaviour::CULL_CUSTOM ? modelRenderer->GetCustomBoundingSphere() : model->GetBoundingSphere();

					std::vector<OvRendering::Resources::Mesh*> meshes;

					if (frustum)
					{
						PROFILER_SPY("Frustum Culling");
						meshes = frustum.value().GetMeshesInFrustum(*model, modelBoundingSphere, transform, cullingOptions);
					}
					else
					{
						meshes = model->GetMeshes();
					}

					if (!meshes.empty())
					{
						float distanceToActor = OvMaths::FVector3::Distance(transform.GetWorldPosition(), camera.GetPosition());
						const OvCore::ECS::Components::CMaterialRenderer::MaterialList& materials = materialRenderer->GetMaterials();

						for (const auto& mesh : meshes)
						{
							OvTools::Utils::OptRef<OvRendering::Data::Material> material;

							if (mesh->GetMaterialIndex() < kMaxMaterialCount)
							{
								if (overrideMaterial && overrideMaterial->IsValid())
								{
									material = overrideMaterial.value();
								}
								else
								{
									material = materials.at(mesh->GetMaterialIndex());
								}

								const bool isMaterialValid = material && material->IsValid();
								const bool hasValidFallbackMaterial = fallbackMaterial && fallbackMaterial->IsValid();

								if (!isMaterialValid && hasValidFallbackMaterial)
								{
									material = fallbackMaterial;
								}
							}

							if (material && material->IsValid())
							{
								OvRendering::Entities::Drawable drawable;
								drawable.mesh = *mesh;
								drawable.material = material;
								drawable.stateMask = material->GenerateStateMask();

								drawable.AddDescriptor<EngineDrawableDescriptor>({
									transform.GetWorldMatrix(),
									materialRenderer->GetUserMatrix()
								});

								if (material->IsBlendable())
								{
									transparents.emplace(distanceToActor, drawable);
								}
								else
								{
									opaques.emplace(distanceToActor, drawable);
								}
							}
						}
					}
				}
			}
		}
	}

	return { opaques, transparents };
}
