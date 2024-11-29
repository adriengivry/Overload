/**
* @project: Overload
* @author: Overload Tech.
* @licence: MIT
*/

#include "OvCore/Rendering/PostProcess/AutoExposureEffect.h"
#include <OvCore/Global/ServiceLocator.h>
#include <OvCore/ResourceManagement/ShaderManager.h>

constexpr uint32_t kLuminanceBufferResolution = 1024;
constexpr uint32_t kExposureBufferResolution = 1;

OvCore::Rendering::PostProcess::AutoExposureEffect::AutoExposureEffect(OvRendering::Core::CompositeRenderer& p_renderer) : AEffect(p_renderer)
{
	m_luminanceMaterial.SetShader(OVSERVICE(OvCore::ResourceManagement::ShaderManager)[":Shaders\\PostProcess\\Luminance.ovfx"]);
	m_exposureMaterial.SetShader(OVSERVICE(OvCore::ResourceManagement::ShaderManager)[":Shaders\\PostProcess\\AutoExposure.ovfx"]);
	m_compensationMaterial.SetShader(OVSERVICE(OvCore::ResourceManagement::ShaderManager)[":Shaders\\PostProcess\\ApplyExposure.ovfx"]);

	for (auto& buffer : m_exposurePingPongBuffer)
	{
		buffer.Resize(kExposureBufferResolution, kExposureBufferResolution);
	}
}

void OvCore::Rendering::PostProcess::AutoExposureEffect::Draw(
	OvRendering::Data::PipelineState p_pso,
	OvRendering::Buffers::Framebuffer& p_src,
	OvRendering::Buffers::Framebuffer& p_dst,
	const EffectSettings& p_settings
)
{
	const auto& autoExposureSettings = static_cast<const AutoExposureSettings&>(p_settings);

	// Luminance calculation
	m_luminanceBuffer.Resize(kLuminanceBufferResolution, kLuminanceBufferResolution);
	m_renderer.Blit(p_pso, p_src, m_luminanceBuffer, m_luminanceMaterial,
		OvRendering::Settings::EBlitFlags::DEFAULT & ~OvRendering::Settings::EBlitFlags::RESIZE_DST_TO_MATCH_SRC);
	m_luminanceBuffer.GenerateMipMaps();

	float elapsedTime = 1.0f;
	auto currentTime = std::chrono::high_resolution_clock::now();
	if (m_previousTime.has_value())
	{
		elapsedTime = std::chrono::duration_cast<std::chrono::duration<float>>(
			currentTime - m_previousTime.value()
		).count();
	}
	m_previousTime = currentTime;

	auto& previousExposure = m_exposurePingPongBuffer[(m_exposurePingPongIndex + 1) % 2];
	auto& currentExposure = m_exposurePingPongBuffer[m_exposurePingPongIndex];
	m_exposurePingPongIndex = (m_exposurePingPongIndex + 1) % 2;

	// Exposure adaptation
	m_exposureMaterial.Set("_LuminanceTexture", m_luminanceBuffer.GetTexture(), true);
	m_exposureMaterial.Set("_MinLuminanceEV", autoExposureSettings.minLuminanceEV, true);
	m_exposureMaterial.Set("_MaxLuminanceEV", autoExposureSettings.maxLuminanceEV, true);
	m_exposureMaterial.Set("_ExposureCompensationEV", autoExposureSettings.exposureCompensationEV, true);
	m_exposureMaterial.Set("_ElapsedTime", elapsedTime, true);
	m_exposureMaterial.Set("_Progressive", static_cast<int>(autoExposureSettings.progressive), true);
	m_exposureMaterial.Set("_SpeedUp", autoExposureSettings.speedUp, true);
	m_exposureMaterial.Set("_SpeedDown", autoExposureSettings.speedDown, true);
	m_renderer.Blit(p_pso, previousExposure, currentExposure, m_exposureMaterial);

	// Apply the exposure to the final image
	m_compensationMaterial.Set("_ExposureTexture", currentExposure.GetTexture(), true);
	m_renderer.Blit(p_pso, p_src, p_dst, m_compensationMaterial);
}
