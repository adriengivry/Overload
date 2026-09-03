/**
* @project: Overload
* @author: Overload Tech.
* @licence: MIT
*/

#pragma once

#include <array>
#include <cstdint>
#include <optional>
#include <string>
#include <vector>

#include <OvCore/ECS/Components/AComponent.h>
#include <OvMaths/FMatrix4.h>
#include <OvMaths/FQuaternion.h>
#include <OvMaths/FVector3.h>

namespace OvCore::ECS { class Actor; }
namespace OvRendering::Resources { class Model; }

namespace OvCore::ECS::Components
{
	constexpr uint32_t kMaxAnimationLayers = 4;

	/**
	* Component responsible for skeletal animation playback and skinning data generation.
	* Up to kMaxAnimationLayers animations can play simultaneously, each layer owning its own
	* animation source model, animation, playback time, speed, loop mode and weight.
	* Bones are blended per node, and only layers holding a track for a given node contribute to it,
	* their weights being normalized against each other. Nodes no layer animates keep their bind pose.
	*/
	class CSkinnedMeshRenderer : public AComponent
	{
	public:
		/**
		* Constructor
		* @param p_owner
		*/
		CSkinnedMeshRenderer(ECS::Actor& p_owner);

		/**
		* Returns the name of the component
		*/
		std::string GetName() override;

		/**
		* Returns the type name of the component
		*/
		virtual std::string GetTypeName() override;

		/**
		* Called by the model renderer when its model changes
		*/
		void NotifyModelChanged();

		/**
		* Returns true if the component has a valid skinning palette ready for rendering
		*/
		bool HasSkinningData() const;

		/**
		* Returns the number of active animation layers (always at least 1)
		*/
		uint32_t GetLayerCount() const;

		/**
		* Returns the maximum number of animation layers a renderer can hold
		*/
		uint32_t GetMaxLayerCount() const;

		/**
		* Appends a new animation layer, returning its index.
		* Returns std::nullopt when the maximum layer count is already reached.
		*/
		std::optional<uint32_t> AddLayer();

		/**
		* Removes an animation layer, shifting the following layers down by one.
		* Fails when the index is invalid or when only one layer remains.
		* @param p_layer
		*/
		bool RemoveLayer(uint32_t p_layer);

		/**
		* Start animation playback on a layer
		* @param p_layer
		*/
		void Play(uint32_t p_layer = 0);

		/**
		* Pause animation playback on a layer
		* @param p_layer
		*/
		void Pause(uint32_t p_layer = 0);

		/**
		* Stop animation playback on a layer and reset its time to 0
		* @param p_layer
		*/
		void Stop(uint32_t p_layer = 0);

		/**
		* Returns true if playback is active on a layer
		* @param p_layer
		*/
		bool IsPlaying(uint32_t p_layer = 0) const;

		/**
		* Sets loop mode on a layer
		* @param p_value
		* @param p_layer
		*/
		void SetLooping(bool p_value, uint32_t p_layer = 0);

		/**
		* Returns true if loop mode is enabled on a layer
		* @param p_layer
		*/
		bool IsLooping(uint32_t p_layer = 0) const;

		/**
		* Set playback speed on a layer
		* @param p_value
		* @param p_layer
		*/
		void SetPlaybackSpeed(float p_value, uint32_t p_layer = 0);

		/**
		* Get playback speed of a layer
		* @param p_layer
		*/
		float GetPlaybackSpeed(uint32_t p_layer = 0) const;

		/**
		* Sets the blend weight of a layer, clamped to the [0,1] range
		* Weights are normalized per node against the other layers animating that node, so a layer
		* animating a node alone contributes fully until its weight reaches 0, where the node falls
		* back to its bind pose
		* @param p_value
		* @param p_layer
		*/
		void SetLayerWeight(float p_value, uint32_t p_layer = 0);

		/**
		* Returns the blend weight of a layer
		* @param p_layer
		*/
		float GetLayerWeight(uint32_t p_layer = 0) const;

		/**
		* Returns the scale applied to mesh bounds during frustum culling
		* Returned value is always >= 1.0f
		*/
		float GetMeshBoundsScale() const;

		/**
		* Sets the scale applied to mesh bounds during frustum culling
		* Any value below 1.0f will be clamped to 1.0f
		* @param p_scale
		*/
		void SetMeshBoundsScale(float p_scale);

		/**
		* Sets the current playback time of a layer in seconds
		* @param p_timeSeconds
		* @param p_layer
		*/
		void SetTime(float p_timeSeconds, uint32_t p_layer = 0);

		/**
		* Returns the current playback time of a layer in seconds
		* @param p_layer
		*/
		float GetTime(uint32_t p_layer = 0) const;

		/**
		* Sets the external model used as animation source by a layer.
		* Pass nullptr to use the rendered model animations.
		* @param p_model
		* @param p_layer
		*/
		void SetAnimationSourceModel(OvRendering::Resources::Model* p_model, uint32_t p_layer = 0);

		/**
		* Returns the external animation source model of a layer, or nullptr when the rendered model is used
		* @param p_layer
		*/
		OvRendering::Resources::Model* GetAnimationSourceModel(uint32_t p_layer = 0) const;

		/**
		* Returns true if the animation source of a layer can be applied to the rendered model skeleton
		* @param p_layer
		*/
		bool IsAnimationSourceCompatible(uint32_t p_layer = 0) const;

		/**
		* Returns the number of animations available to a layer
		* @param p_layer
		*/
		uint32_t GetAnimationCount(uint32_t p_layer = 0) const;

		/**
		* Returns the animation name at index for a layer (std::nullopt if index is invalid)
		* @param p_index
		* @param p_layer
		*/
		std::optional<std::string> GetAnimationName(uint32_t p_index, uint32_t p_layer = 0) const;

		/**
		* Sets the active animation of a layer by index. Pass std::nullopt to clear the layer.
		* @param p_index
		* @param p_layer
		*/
		bool SetAnimation(std::optional<uint32_t> p_index, uint32_t p_layer = 0);

		/**
		* Sets the active animation of a layer by name
		* @param p_name
		* @param p_layer
		*/
		bool SetAnimation(const std::string& p_name, uint32_t p_layer = 0);

		/**
		* Returns the active animation index of a layer, or std::nullopt if none is set
		* @param p_layer
		*/
		std::optional<uint32_t> GetActiveAnimationIndex(uint32_t p_layer = 0) const;

		/**
		* Returns the active animation name of a layer (std::nullopt if none)
		* @param p_layer
		*/
		std::optional<std::string> GetActiveAnimationName(uint32_t p_layer = 0) const;

		/**
		* Returns the number of available bones
		*/
		uint32_t GetBoneCount() const;

		/**
		* Returns the bone name at index (std::nullopt if index is invalid)
		* @param p_index
		*/
		std::optional<std::string> GetBoneName(uint32_t p_index) const;

		/**
		* Returns the bone index by name (std::nullopt if not found)
		* @param p_name
		*/
		std::optional<uint32_t> GetBoneIndex(const std::string& p_name) const;

		/**
		* Returns the local bone position (std::nullopt if index is invalid)
		* @param p_boneIndex
		*/
		std::optional<OvMaths::FVector3> GetBoneLocalPosition(uint32_t p_boneIndex) const;

		/**
		* Returns the local bone rotation (std::nullopt if index is invalid)
		* @param p_boneIndex
		*/
		std::optional<OvMaths::FQuaternion> GetBoneLocalRotation(uint32_t p_boneIndex) const;

		/**
		* Returns the local bone scale (std::nullopt if index is invalid)
		* @param p_boneIndex
		*/
		std::optional<OvMaths::FVector3> GetBoneLocalScale(uint32_t p_boneIndex) const;

		/**
		* Sets the local bone position
		* @param p_boneIndex
		* @param p_position
		*/
		bool SetBoneLocalPosition(uint32_t p_boneIndex, const OvMaths::FVector3& p_position);

		/**
		* Sets the local bone rotation
		* @param p_boneIndex
		* @param p_rotation
		*/
		bool SetBoneLocalRotation(uint32_t p_boneIndex, const OvMaths::FQuaternion& p_rotation);

		/**
		* Sets the local bone scale
		* @param p_boneIndex
		* @param p_scale
		*/
		bool SetBoneLocalScale(uint32_t p_boneIndex, const OvMaths::FVector3& p_scale);

		/**
		* Returns the transposed skinning matrix palette ready for GPU upload
		*/
		const std::vector<OvMaths::FMatrix4>& GetBoneMatricesTransposed() const;

		/**
		* Returns an incrementing pose version, updated whenever bone matrices change
		*/
		uint64_t GetPoseVersion() const;

		/**
		* Called each frame by the actor
		* @param p_deltaTime
		*/
		void OnUpdate(float p_deltaTime) override;

		/**
		* Serialize the component
		* @param p_doc
		* @param p_node
		*/
		virtual void OnSerialize(tinyxml2::XMLDocument& p_doc, tinyxml2::XMLNode* p_node) override;

		/**
		* Deserialize the component
		* @param p_doc
		* @param p_node
		*/
		virtual void OnDeserialize(tinyxml2::XMLDocument& p_doc, tinyxml2::XMLNode* p_node) override;

		/**
		* Draw component inspector widgets
		* @param p_root
		*/
		virtual void OnInspector(OvUI::Internal::WidgetContainer& p_root) override;

	private:
		struct AnimationLayer
		{
			OvRendering::Resources::Model* animationSourceModel = nullptr;
			std::optional<uint32_t> animationIndex = std::nullopt;
			std::string deserializedAnimationName;
			float timeTicks = 0.0f;
			float speed = 1.0f;
			float weight = 1.0f;
			bool playing = true;
			bool looping = true;

			// Runtime data, rebuilt whenever the model or the layer animation source changes.
			// sourceNodeByTargetNode maps a rendered skeleton node to the source node driving it
			// (-1 when unanimated), and is empty when the source is incompatible.
			std::vector<std::string> animationNames;
			std::vector<int32_t> sourceNodeByTargetNode;
		};

		bool HasCompatibleModel() const;
		bool IsLayerCompatible(const AnimationLayer& p_layer) const;
		const OvRendering::Resources::Model* GetLayerAnimationModel(const AnimationLayer& p_layer) const;
		AnimationLayer* FindLayer(uint32_t p_layer);
		const AnimationLayer* FindLayer(uint32_t p_layer) const;
		void SyncWithModel();
		void RebuildRuntimeData();
		void RebuildLayerRuntimeData(AnimationLayer& p_layer, std::vector<int32_t>& p_nodeMapScratch);
		void ResolveLayerAnimation(AnimationLayer& p_layer);
		void EvaluatePose();
		std::optional<uint32_t> GetNodeIndexFromBoneIndex(uint32_t p_boneIndex) const;
		void RecomputeBoneMatricesFromLocalPose();
		float GetAnimationDurationSeconds(uint32_t p_layer) const;
		void UpdatePlayback(AnimationLayer& p_layer, float p_deltaTime);
		void BuildLayerWidgets(OvUI::Internal::WidgetContainer& p_container);

	private:
		const OvRendering::Resources::Model* m_model = nullptr;

		float m_meshBoundsScale = 1.5f;
		float m_poseEvaluationRate = 60.0f;
		float m_poseEvaluationAccumulator = 0.0f;

		std::array<AnimationLayer, kMaxAnimationLayers> m_layers;
		uint32_t m_layerCount = 1;

		uint64_t m_poseVersion = 0;
		bool m_manualPoseOverride = false;

		std::vector<OvMaths::FMatrix4> m_localPose;
		std::vector<OvMaths::FMatrix4> m_globalPose;
		std::vector<OvMaths::FMatrix4> m_boneMatrices;
		std::vector<OvMaths::FMatrix4> m_boneMatricesTransposed;
	};

	template<>
	struct ComponentTraits<OvCore::ECS::Components::CSkinnedMeshRenderer>
	{
		static constexpr std::string_view Name = "class OvCore::ECS::Components::CSkinnedMeshRenderer";
	};
}
