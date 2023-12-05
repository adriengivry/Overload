/**
* @project: Overload
* @author: Overload Tech.
* @licence: MIT
*/

#pragma once

#include <map>

#include <OvRendering/Core/CompositeRenderer.h>
#include <OvRendering/Resources/Mesh.h>
#include <OvRendering/Data/Frustum.h>
#include <OvRendering/Entities/Drawable.h>
#include <OvRendering/Buffers/UniformBuffer.h>
#include <OvRendering/Buffers/ShaderStorageBuffer.h>

#include "OvCore/Resources/Material.h"
#include "OvCore/ECS/Actor.h"
#include "OvCore/ECS/Components/CCamera.h"
#include "OvCore/SceneSystem/Scene.h"

namespace OvCore::Rendering
{
	/**
	* Extension of the ImmediateRenderer adding support for the scene system
	*/
	class SceneRenderer : public OvRendering::Core::CompositeRenderer
	{
	public:
		using OpaqueDrawables = std::multimap<float, OvRendering::Entities::Drawable, std::less<float>>;
		using TransparentDrawables = std::multimap<float, OvRendering::Entities::Drawable, std::greater<float>>;

		struct AllDrawables
		{
			OpaqueDrawables opaques;
			TransparentDrawables transparents;
		};

		struct SceneDescriptor
		{
			OvCore::SceneSystem::Scene& scene;
			std::optional<OvRendering::Data::Frustum> frustumOverride;
			std::optional<std::reference_wrapper<OvCore::Resources::Material>> materialOverride;
		};

		/**
		* Constructor of the Renderer
		* @param p_driver
		*/
		SceneRenderer(OvRendering::Context::Driver& p_driver);

		/**
		* Begin Frame
		* @param p_frameDescriptor
		*/
		virtual void BeginFrame(const OvRendering::Data::FrameDescriptor& p_frameDescriptor) override;

	protected:
		AllDrawables ParseScene();
	};
}
