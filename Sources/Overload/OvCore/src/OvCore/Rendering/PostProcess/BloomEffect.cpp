/**
* @project: Overload
* @author: Overload Tech.
* @licence: MIT
*/

#include <OvCore/Rendering/PostProcess/BloomEffect.h>
#include <OvCore/Global/ServiceLocator.h>
#include <OvCore/ResourceManagement/ShaderManager.h>

OvCore::Rendering::PostProcess::BloomEffect::BloomEffect(OvRendering::Core::CompositeRenderer& p_renderer) : AEffect(p_renderer)
{
	auto& shaderManager = OVSERVICE(OvCore::ResourceManagement::ShaderManager);

	m_brightnessMaterial.SetShader(shaderManager[":Shaders\\PostProcess\\Brightness.ovfx"]);
	m_blurMaterial.SetShader(shaderManager[":Shaders\\PostProcess\\Blur.ovfx"]);
	m_bloomMaterial.SetShader(shaderManager[":Shaders\\PostProcess\\Bloom.ovfx"]);

	m_brightnessMaterial.SetDepthTest(false);
	m_brightnessMaterial.SetDepthWriting(false);

	m_blurMaterial.SetDepthTest(false);
	m_blurMaterial.SetDepthWriting(false);

	m_bloomMaterial.SetDepthTest(false);
	m_bloomMaterial.SetDepthWriting(false);
}

bool OvCore::Rendering::PostProcess::BloomEffect::IsApplicable(const EffectSettings& p_settings) const
{
	auto& bloomSettings = static_cast<const BloomSettings&>(p_settings);

	return
		AEffect::IsApplicable(p_settings) &&
		bloomSettings.intensity > 0.0f;
}

void OvCore::Rendering::PostProcess::BloomEffect::Draw(
	OvRendering::Data::PipelineState p_pso,
	OvRendering::Buffers::Framebuffer& p_src,
	OvRendering::Buffers::Framebuffer& p_dst,
	const EffectSettings& p_settings
)
{
	const auto& bloomSettings = static_cast<const BloomSettings&>(p_settings);

	// Step 1: Extract bright spots from the source
	m_bloomPingPong[1].Resize(p_src.GetWidth(), p_src.GetHeight());

	m_brightnessMaterial.Set("_InputTexture", p_src.GetTexture(), true);
	m_brightnessMaterial.Set("_Threshold", bloomSettings.threshold, true);

	m_renderer.Blit(p_pso, p_src, m_bloomPingPong[0], m_brightnessMaterial);

	// Step 2: Apply gaussian blur on bright spots (horizontal and vertical)
	bool horizontal = true;

	for (int i = 0; i < 10; ++i) { // Perform 10 blur passes (alternating)
		auto& currentSrc = horizontal ? m_bloomPingPong[0] : m_bloomPingPong[1];
		auto& currentDst = horizontal ? m_bloomPingPong[1] : m_bloomPingPong[0];

		m_blurMaterial.Set("_InputTexture", currentSrc.GetTexture(), true);
		m_blurMaterial.Set("_Horizontal", horizontal ? true : false, true);
		m_blurMaterial.Set("_BlurSize", bloomSettings.radius, true);
		m_blurMaterial.Set("_KernelSize", bloomSettings.kernelSize, true);

		m_renderer.Blit(p_pso, currentSrc, currentDst, m_blurMaterial);

		horizontal = !horizontal;
	}

	// Step 3: Combine bloom with original framebuffer
	m_bloomMaterial.Set("_InputTexture", p_src.GetTexture(), true);
	m_bloomMaterial.Set("_BloomTexture", m_bloomPingPong[0].GetTexture(), true);
	m_bloomMaterial.Set("_BloomIntensity", bloomSettings.intensity, true);

	m_renderer.Blit(p_pso, m_bloomPingPong[0], p_dst, m_bloomMaterial);
}
