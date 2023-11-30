/**
* @project: Overload
* @author: Overload Tech.
* @licence: MIT
*/

#pragma once

#include <OvRendering/Entities/Camera.h>
#include <OvRendering/Features/DebugShapeRenderFeature.h>

#include <OvCore/ECS/Actor.h>
#include <OvCore/SceneSystem/SceneManager.h>
#include <OvCore/ECS/Components/CModelRenderer.h>
#include <OvCore/Resources/Material.h>
#include <OvCore/ECS/Components/CAmbientBoxLight.h>
#include <OvCore/ECS/Components/CAmbientSphereLight.h>
#include <OvCore/Rendering/SceneRenderer.h>

#include "OvEditor/Core/Context.h"

namespace OvEditor::Rendering
{
	/**
	* Draw a grid
	*/
	class GridRenderFeature : public OvRendering::Features::ARenderFeature
	{
	public:
		struct GridDescriptor
		{
			OvMaths::FVector3 gridColor;
			OvMaths::FVector3 viewPosition;
		};

		/**
		* Constructor
		* @param p_renderer
		*/
		GridRenderFeature(OvRendering::Core::CompositeRenderer& p_renderer);

		/**
		* Returns the render pass mask used by the grid
		*/
		virtual OvRendering::Settings::ERenderPassMask GetRenderPassMask() const;

		/**
		* Draw the grid
		* @param p_pass
		*/
		virtual void DrawPass(OvRendering::Settings::ERenderPass p_pass) override;

	private:
		OvCore::Resources::Material m_gridMaterial;
	};
}