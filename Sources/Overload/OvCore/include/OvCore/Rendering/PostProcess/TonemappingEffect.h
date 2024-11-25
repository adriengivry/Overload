/**
* @project: Overload
* @author: Overload Tech.
* @licence: MIT
*/

#pragma once

#include <OvCore/Rendering/PostProcess/AEffect.h>
#include <OvRendering/Data/Material.h>

namespace OvCore::Rendering::PostProcess
{
	struct TonemappingSettings : public EffectSettings
	{
		float exposure = 1.4f;
	};

	/**
	* Post-processing effect that applies Tonemapping
	* Remap HDR values to the LDR range.
	*/
	class TonemappingEffect : public AEffect
	{
	public:
		/**
		* Constructor of the TonemappingEffect class
		* @param p_renderer
		*/
		TonemappingEffect(OvRendering::Core::CompositeRenderer& p_renderer);

		/**
		* Render the Tonemapping effect
		* @note: make sure the effect is applicable before calling this method
		* @param p_pso
		* @param p_src
		* @param p_dst
		* @param p_settings
		*/
		virtual void Draw(
			OvRendering::Data::PipelineState p_pso,
			OvRendering::Buffers::Framebuffer& p_src,
			OvRendering::Buffers::Framebuffer& p_dst,
			const EffectSettings& p_settings
		) override;

	private:
		OvRendering::Data::Material m_material;
	};
}