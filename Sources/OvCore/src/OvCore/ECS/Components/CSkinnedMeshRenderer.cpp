/**
* @project: Overload
* @author: Overload Tech.
* @licence: MIT
*/

#include <algorithm>
#include <cmath>
#include <limits>

#include <optional>
#include <string>
#include <tinyxml2.h>

#include <OvCore/ECS/Actor.h>
#include <OvCore/ECS/Components/CModelRenderer.h>
#include <OvCore/ECS/Components/CSkinnedMeshRenderer.h>
#include <OvCore/Global/ServiceLocator.h>
#include <OvCore/Helpers/GUIDrawer.h>
#include <OvCore/Helpers/Serializer.h>
#include <OvCore/ResourceManagement/ModelManager.h>
#include <OvDebug/Logger.h>
#include <OvMaths/FMatrix3.h>
#include <OvMaths/FTransform.h>
#include <OvUI/Plugins/DataDispatcher.h>
#include <OvUI/Styling/Style.h>
#include <OvUI/Widgets/Buttons/Button.h>
#include <OvUI/Widgets/Layout/Columns.h>
#include <OvUI/Widgets/Layout/Group.h>
#include <OvUI/Widgets/Layout/TreeNode.h>
#include <OvUI/Widgets/Selection/ComboBox.h>
#include <OvUI/Widgets/Texts/TextColored.h>

namespace
{
	constexpr float kMinimumLayerWeight = 0.0001f;

	// std::clamp propagates NaN, so non-finite weights are rejected before clamping
	float ClampLayerWeight(float p_value)
	{
		return std::isfinite(p_value) ? std::clamp(p_value, 0.0f, 1.0f) : 0.0f;
	}

	float WrapTime(float p_value, float p_duration)
	{
		if (p_duration <= 0.0f)
		{
			return 0.0f;
		}

		const auto wrapped = std::fmod(p_value, p_duration);
		return wrapped < 0.0f ? wrapped + p_duration : wrapped;
	}

	template<typename T, typename TLerp>
	T SampleKeys(
		const std::vector<OvRendering::Animation::Keyframe<T>>& p_keys,
		float p_time,
		float p_duration,
		const T& p_defaultValue,
		bool p_looping,
		TLerp p_lerp
	)
	{
		if (p_keys.empty())
		{
			return p_defaultValue;
		}

		if (p_keys.size() == 1)
		{
			return p_keys.front().value;
		}

		if (!p_looping)
		{
			if (p_time <= p_keys.front().time) return p_keys.front().value;
			if (p_time >= p_keys.back().time)  return p_keys.back().value;
		}

		const auto nextIt = std::upper_bound(
			p_keys.begin(),
			p_keys.end(),
			p_time,
			[](float p_lhs, const auto& p_rhs) { return p_lhs < p_rhs.time; }
		);

		const auto interpolate = [&](const auto& p_prev, const auto& p_next, float p_segmentDuration)
		{
			if (p_segmentDuration <= std::numeric_limits<float>::epsilon())
			{
				return p_prev.value;
			}

			const float alpha = std::clamp((p_time - p_prev.time) / p_segmentDuration, 0.0f, 1.0f);
			return p_lerp(p_prev.value, p_next.value, alpha);
		};

		if (nextIt == p_keys.end())
		{
			if (!p_looping)
			{
				return p_keys.back().value;
			}

			const auto& prev = p_keys.back();
			const auto& next = p_keys.front();
			return interpolate(prev, next, (p_duration - prev.time) + next.time);
		}

		if (nextIt == p_keys.begin())
		{
			return nextIt->value;
		}

		const auto& prev = *std::prev(nextIt);
		const auto& next = *nextIt;
		return interpolate(prev, next, next.time - prev.time);
	}

	void DecomposeLocalTransform(
		const OvMaths::FMatrix4& p_matrix,
		OvMaths::FVector3& p_position,
		OvMaths::FQuaternion& p_rotation,
		OvMaths::FVector3& p_scale
	)
	{
		p_position.x = p_matrix.data[3];
		p_position.y = p_matrix.data[7];
		p_position.z = p_matrix.data[11];

		OvMaths::FVector3 columns[3] =
		{
			{ p_matrix.data[0], p_matrix.data[4], p_matrix.data[8] },
			{ p_matrix.data[1], p_matrix.data[5], p_matrix.data[9] },
			{ p_matrix.data[2], p_matrix.data[6], p_matrix.data[10] }
		};

		p_scale.x = OvMaths::FVector3::Length(columns[0]);
		p_scale.y = OvMaths::FVector3::Length(columns[1]);
		p_scale.z = OvMaths::FVector3::Length(columns[2]);

		if (p_scale.x > 0.0f)
		{
			columns[0] /= p_scale.x;
		}

		if (p_scale.y > 0.0f)
		{
			columns[1] /= p_scale.y;
		}

		if (p_scale.z > 0.0f)
		{
			columns[2] /= p_scale.z;
		}

		const OvMaths::FMatrix3 rotationMatrix(
			columns[0].x, columns[1].x, columns[2].x,
			columns[0].y, columns[1].y, columns[2].y,
			columns[0].z, columns[1].z, columns[2].z
		);

		p_rotation = OvMaths::FQuaternion(rotationMatrix);
	}

	bool AreMatricesClose(const OvMaths::FMatrix4& p_left, const OvMaths::FMatrix4& p_right)
	{
		constexpr float kSkeletonTransformTolerance = 0.0001f;

		for (uint8_t i = 0; i < 16; ++i)
		{
			if (std::abs(p_left.data[i] - p_right.data[i]) > kSkeletonTransformTolerance)
			{
				return false;
			}
		}

		return true;
	}

	bool CollectRequiredSkinningNodes(
		const OvRendering::Animation::Skeleton& p_skeleton,
		std::vector<bool>& p_requiredNodes
	)
	{
		p_requiredNodes.assign(p_skeleton.nodes.size(), false);

		for (const auto& bone : p_skeleton.bones)
		{
			if (bone.nodeIndex >= p_skeleton.nodes.size())
			{
				return false;
			}

			int32_t nodeIndex = static_cast<int32_t>(bone.nodeIndex);
			while (nodeIndex >= 0)
			{
				const auto currentNodeIndex = static_cast<size_t>(nodeIndex);
				if (currentNodeIndex >= p_skeleton.nodes.size())
				{
					return false;
				}

				if (p_requiredNodes[currentNodeIndex])
				{
					break;
				}

				p_requiredNodes[currentNodeIndex] = true;
				nodeIndex = p_skeleton.nodes[currentNodeIndex].parentIndex;
			}
		}

		return true;
	}

	bool BuildAnimationNodeMap(
		const OvRendering::Animation::Skeleton& p_targetSkeleton,
		const OvRendering::Animation::Skeleton& p_sourceSkeleton,
		std::vector<int32_t>& p_nodeMap
	)
	{
		p_nodeMap.assign(p_sourceSkeleton.nodes.size(), -1);

		if (p_targetSkeleton.nodes.empty() || p_sourceSkeleton.nodes.empty() || p_targetSkeleton.bones.empty())
		{
			return false;
		}

		std::vector<bool> requiredNodes;
		if (!CollectRequiredSkinningNodes(p_targetSkeleton, requiredNodes))
		{
			return false;
		}

		for (size_t targetNodeIndex = 0; targetNodeIndex < requiredNodes.size(); ++targetNodeIndex)
		{
			if (!requiredNodes[targetNodeIndex])
			{
				continue;
			}

			const auto& targetNode = p_targetSkeleton.nodes[targetNodeIndex];
			const auto sourceNodeIndex = p_sourceSkeleton.FindNodeIndex(targetNode.name);
			if (!sourceNodeIndex.has_value() || *sourceNodeIndex >= p_sourceSkeleton.nodes.size())
			{
				return false;
			}

			const auto& sourceNode = p_sourceSkeleton.nodes[*sourceNodeIndex];
			const int32_t targetParentIndex = targetNode.parentIndex;
			const int32_t sourceParentIndex = sourceNode.parentIndex;
			const bool hasRequiredTargetParent =
				targetParentIndex >= 0 &&
				static_cast<size_t>(targetParentIndex) < requiredNodes.size() &&
				requiredNodes[static_cast<size_t>(targetParentIndex)];

			if (targetParentIndex >= 0 && static_cast<size_t>(targetParentIndex) >= requiredNodes.size())
			{
				return false;
			}

			if (hasRequiredTargetParent)
			{
				if (
					sourceParentIndex < 0 ||
					static_cast<size_t>(sourceParentIndex) >= p_sourceSkeleton.nodes.size() ||
					p_sourceSkeleton.nodes[static_cast<size_t>(sourceParentIndex)].name != p_targetSkeleton.nodes[static_cast<size_t>(targetParentIndex)].name
				)
				{
					return false;
				}
			}
			else if (sourceParentIndex >= 0)
			{
				return false;
			}

			if (!AreMatricesClose(targetNode.localBindTransform, sourceNode.localBindTransform))
			{
				return false;
			}

			p_nodeMap[*sourceNodeIndex] = static_cast<int32_t>(targetNodeIndex);
		}

		if (!p_sourceSkeleton.bones.empty())
		{
			if (p_sourceSkeleton.bones.size() != p_targetSkeleton.bones.size())
			{
				return false;
			}

			for (const auto& targetBone : p_targetSkeleton.bones)
			{
				const auto sourceBoneIndex = p_sourceSkeleton.FindBoneIndex(targetBone.name);
				if (!sourceBoneIndex.has_value() || *sourceBoneIndex >= p_sourceSkeleton.bones.size())
				{
					return false;
				}

				const auto& sourceBone = p_sourceSkeleton.bones[*sourceBoneIndex];
				if (
					sourceBone.nodeIndex >= p_nodeMap.size() ||
					p_nodeMap[sourceBone.nodeIndex] != static_cast<int32_t>(targetBone.nodeIndex)
				)
				{
					return false;
				}
			}
		}

		return true;
	}
}

OvCore::ECS::Components::CSkinnedMeshRenderer::CSkinnedMeshRenderer(ECS::Actor& p_owner) :
	AComponent(p_owner)
{
	m_layers.emplace_back();

	NotifyModelChanged();
}

std::string OvCore::ECS::Components::CSkinnedMeshRenderer::GetName()
{
	return "Skinned Mesh Renderer";
}

std::string OvCore::ECS::Components::CSkinnedMeshRenderer::GetTypeName()
{
	return std::string{ ComponentTraits<CSkinnedMeshRenderer>::Name };
}

void OvCore::ECS::Components::CSkinnedMeshRenderer::NotifyModelChanged()
{
	m_model = nullptr;
	SyncWithModel();
}

bool OvCore::ECS::Components::CSkinnedMeshRenderer::HasSkinningData() const
{
	const bool hasAnimatedLayer = std::any_of(m_layers.begin(), m_layers.end(), [this](const AnimationLayer& p_layer)
	{
		return p_layer.animationIndex.has_value() && IsLayerCompatible(p_layer);
	});

	return HasCompatibleModel() &&
		!m_boneMatrices.empty() &&
		(hasAnimatedLayer || m_manualPoseOverride);
}

uint32_t OvCore::ECS::Components::CSkinnedMeshRenderer::GetLayerCount() const
{
	return static_cast<uint32_t>(m_layers.size());
}

uint32_t OvCore::ECS::Components::CSkinnedMeshRenderer::AddLayer()
{
	m_layers.emplace_back();
	RebuildRuntimeData();
	return static_cast<uint32_t>(m_layers.size() - 1);
}

bool OvCore::ECS::Components::CSkinnedMeshRenderer::RemoveLayer(uint32_t p_layer)
{
	if (p_layer >= m_layers.size() || m_layers.size() <= 1)
	{
		return false;
	}

	m_layers.erase(m_layers.begin() + p_layer);
	RebuildRuntimeData();
	return true;
}

void OvCore::ECS::Components::CSkinnedMeshRenderer::Play(uint32_t p_layer)
{
	const auto layer = FindLayer(p_layer);
	if (!layer)
	{
		return;
	}

	layer->playing = true;
	m_poseEvaluationAccumulator = 0.0f;
}

void OvCore::ECS::Components::CSkinnedMeshRenderer::Pause(uint32_t p_layer)
{
	const auto layer = FindLayer(p_layer);
	if (!layer)
	{
		return;
	}

	layer->playing = false;
	m_poseEvaluationAccumulator = 0.0f;
}

void OvCore::ECS::Components::CSkinnedMeshRenderer::Stop(uint32_t p_layer)
{
	const auto layer = FindLayer(p_layer);
	if (!layer)
	{
		return;
	}

	layer->playing = false;
	layer->timeTicks = 0.0f;
	m_poseEvaluationAccumulator = 0.0f;
	m_manualPoseOverride = false;
	EvaluatePose();
}

bool OvCore::ECS::Components::CSkinnedMeshRenderer::IsPlaying(uint32_t p_layer) const
{
	const auto layer = FindLayer(p_layer);
	return layer && layer->playing;
}

void OvCore::ECS::Components::CSkinnedMeshRenderer::SetLooping(bool p_value, uint32_t p_layer)
{
	if (const auto layer = FindLayer(p_layer))
	{
		layer->looping = p_value;
	}
}

bool OvCore::ECS::Components::CSkinnedMeshRenderer::IsLooping(uint32_t p_layer) const
{
	const auto layer = FindLayer(p_layer);
	return layer && layer->looping;
}

void OvCore::ECS::Components::CSkinnedMeshRenderer::SetPlaybackSpeed(float p_value, uint32_t p_layer)
{
	if (const auto layer = FindLayer(p_layer))
	{
		layer->speed = p_value;
	}
}

float OvCore::ECS::Components::CSkinnedMeshRenderer::GetPlaybackSpeed(uint32_t p_layer) const
{
	const auto layer = FindLayer(p_layer);
	return layer ? layer->speed : 0.0f;
}

void OvCore::ECS::Components::CSkinnedMeshRenderer::SetLayerWeight(float p_value, uint32_t p_layer)
{
	const auto layer = FindLayer(p_layer);
	if (!layer)
	{
		return;
	}

	layer->weight = ClampLayerWeight(p_value);
	m_poseEvaluationAccumulator = 0.0f;
	EvaluatePose();
}

float OvCore::ECS::Components::CSkinnedMeshRenderer::GetLayerWeight(uint32_t p_layer) const
{
	const auto layer = FindLayer(p_layer);
	return layer ? layer->weight : 0.0f;
}

float OvCore::ECS::Components::CSkinnedMeshRenderer::GetMeshBoundsScale() const
{
	return m_meshBoundsScale;
}

void OvCore::ECS::Components::CSkinnedMeshRenderer::SetMeshBoundsScale(float p_scale)
{
	m_meshBoundsScale = std::max(1.0f, p_scale);
}

void OvCore::ECS::Components::CSkinnedMeshRenderer::SetTime(float p_timeSeconds, uint32_t p_layer)
{
	const auto layer = FindLayer(p_layer);
	if (!layer || !IsLayerCompatible(*layer) || !layer->animationIndex.has_value())
	{
		return;
	}

	const auto& animation = GetLayerAnimationModel(*layer)->GetAnimations().at(*layer->animationIndex);
	const float ticksPerSecond = animation.GetEffectiveTicksPerSecond();

	layer->timeTicks = p_timeSeconds * ticksPerSecond;
	if (layer->looping)
	{
		layer->timeTicks = WrapTime(layer->timeTicks, animation.duration);
	}
	else
	{
		layer->timeTicks = std::clamp(layer->timeTicks, 0.0f, animation.duration);
	}

	m_poseEvaluationAccumulator = 0.0f;
	EvaluatePose();
}

float OvCore::ECS::Components::CSkinnedMeshRenderer::GetTime(uint32_t p_layer) const
{
	const auto layer = FindLayer(p_layer);
	if (!layer || !IsLayerCompatible(*layer) || !layer->animationIndex.has_value())
	{
		return 0.0f;
	}

	const auto& animation = GetLayerAnimationModel(*layer)->GetAnimations().at(*layer->animationIndex);
	const float ticksPerSecond = animation.GetEffectiveTicksPerSecond();
	return layer->timeTicks / ticksPerSecond;
}

void OvCore::ECS::Components::CSkinnedMeshRenderer::SetAnimationSourceModel(OvRendering::Resources::Model* p_model, uint32_t p_layer)
{
	const auto layer = FindLayer(p_layer);
	if (!layer || layer->animationSourceModel == p_model)
	{
		return;
	}

	layer->animationSourceModel = p_model;
	RebuildRuntimeData();
}

OvRendering::Resources::Model* OvCore::ECS::Components::CSkinnedMeshRenderer::GetAnimationSourceModel(uint32_t p_layer) const
{
	const auto layer = FindLayer(p_layer);
	return layer ? layer->animationSourceModel : nullptr;
}

bool OvCore::ECS::Components::CSkinnedMeshRenderer::IsAnimationSourceCompatible(uint32_t p_layer) const
{
	const auto layer = FindLayer(p_layer);
	return layer && IsLayerCompatible(*layer);
}

uint32_t OvCore::ECS::Components::CSkinnedMeshRenderer::GetAnimationCount(uint32_t p_layer) const
{
	const auto layer = FindLayer(p_layer);
	return layer ? static_cast<uint32_t>(layer->animationNames.size()) : 0;
}

std::optional<std::string> OvCore::ECS::Components::CSkinnedMeshRenderer::GetAnimationName(uint32_t p_index, uint32_t p_layer) const
{
	const auto layer = FindLayer(p_layer);
	if (!layer || p_index >= layer->animationNames.size())
	{
		return std::nullopt;
	}

	return layer->animationNames[p_index];
}

bool OvCore::ECS::Components::CSkinnedMeshRenderer::SetAnimation(std::optional<uint32_t> p_index, uint32_t p_layer)
{
	const auto layer = FindLayer(p_layer);
	if (!layer)
	{
		return false;
	}

	if (p_index.has_value() && *p_index >= layer->animationNames.size())
	{
		return false;
	}

	layer->animationIndex = p_index;
	layer->timeTicks = 0.0f;
	m_poseEvaluationAccumulator = 0.0f;
	m_manualPoseOverride = false;
	EvaluatePose();
	return true;
}

bool OvCore::ECS::Components::CSkinnedMeshRenderer::SetAnimation(const std::string& p_name, uint32_t p_layer)
{
	const auto layer = FindLayer(p_layer);
	if (!layer)
	{
		return false;
	}

	const auto& animationNames = layer->animationNames;
	const auto found = std::find(animationNames.begin(), animationNames.end(), p_name);

	if (found == animationNames.end())
	{
		return false;
	}

	return SetAnimation(static_cast<uint32_t>(std::distance(animationNames.begin(), found)), p_layer);
}

std::optional<uint32_t> OvCore::ECS::Components::CSkinnedMeshRenderer::GetActiveAnimationIndex(uint32_t p_layer) const
{
	const auto layer = FindLayer(p_layer);
	return layer ? layer->animationIndex : std::nullopt;
}

std::optional<std::string> OvCore::ECS::Components::CSkinnedMeshRenderer::GetActiveAnimationName(uint32_t p_layer) const
{
	const auto animationIndex = GetActiveAnimationIndex(p_layer);
	if (!animationIndex.has_value())
	{
		return std::nullopt;
	}

	return GetAnimationName(*animationIndex, p_layer);
}

uint32_t OvCore::ECS::Components::CSkinnedMeshRenderer::GetBoneCount() const
{
	if (!HasCompatibleModel())
	{
		return 0;
	}

	return static_cast<uint32_t>(m_model->GetSkeleton().value().bones.size());
}

std::optional<std::string> OvCore::ECS::Components::CSkinnedMeshRenderer::GetBoneName(uint32_t p_index) const
{
	if (!HasCompatibleModel())
	{
		return std::nullopt;
	}

	const auto& bones = m_model->GetSkeleton().value().bones;
	if (p_index >= bones.size())
	{
		return std::nullopt;
	}

	return bones[p_index].name;
}

std::optional<uint32_t> OvCore::ECS::Components::CSkinnedMeshRenderer::GetBoneIndex(const std::string& p_name) const
{
	if (!HasCompatibleModel())
	{
		return std::nullopt;
	}

	return m_model->GetSkeleton().value().FindBoneIndex(p_name);
}

std::optional<OvMaths::FVector3> OvCore::ECS::Components::CSkinnedMeshRenderer::GetBoneLocalPosition(uint32_t p_boneIndex) const
{
	const auto nodeIndex = GetNodeIndexFromBoneIndex(p_boneIndex);
	if (!nodeIndex.has_value())
	{
		return std::nullopt;
	}

	OvMaths::FVector3 position;
	OvMaths::FQuaternion rotation;
	OvMaths::FVector3 scale;
	DecomposeLocalTransform(m_localPose[*nodeIndex], position, rotation, scale);
	return position;
}

std::optional<OvMaths::FQuaternion> OvCore::ECS::Components::CSkinnedMeshRenderer::GetBoneLocalRotation(uint32_t p_boneIndex) const
{
	const auto nodeIndex = GetNodeIndexFromBoneIndex(p_boneIndex);
	if (!nodeIndex.has_value())
	{
		return std::nullopt;
	}

	OvMaths::FVector3 position;
	OvMaths::FQuaternion rotation;
	OvMaths::FVector3 scale;
	DecomposeLocalTransform(m_localPose[*nodeIndex], position, rotation, scale);
	return rotation;
}

std::optional<OvMaths::FVector3> OvCore::ECS::Components::CSkinnedMeshRenderer::GetBoneLocalScale(uint32_t p_boneIndex) const
{
	const auto nodeIndex = GetNodeIndexFromBoneIndex(p_boneIndex);
	if (!nodeIndex.has_value())
	{
		return std::nullopt;
	}

	OvMaths::FVector3 position;
	OvMaths::FQuaternion rotation;
	OvMaths::FVector3 scale;
	DecomposeLocalTransform(m_localPose[*nodeIndex], position, rotation, scale);
	return scale;
}

bool OvCore::ECS::Components::CSkinnedMeshRenderer::SetBoneLocalPosition(uint32_t p_boneIndex, const OvMaths::FVector3& p_position)
{
	const auto nodeIndex = GetNodeIndexFromBoneIndex(p_boneIndex);
	if (!nodeIndex.has_value())
	{
		return false;
	}

	OvMaths::FVector3 currentPosition;
	OvMaths::FQuaternion currentRotation;
	OvMaths::FVector3 currentScale;
	DecomposeLocalTransform(m_localPose[*nodeIndex], currentPosition, currentRotation, currentScale);

	const OvMaths::FTransform transform(p_position, currentRotation, currentScale);
	m_localPose[*nodeIndex] = transform.GetLocalMatrix();
	m_manualPoseOverride = true;
	RecomputeBoneMatricesFromLocalPose();
	return true;
}

bool OvCore::ECS::Components::CSkinnedMeshRenderer::SetBoneLocalRotation(uint32_t p_boneIndex, const OvMaths::FQuaternion& p_rotation)
{
	const auto nodeIndex = GetNodeIndexFromBoneIndex(p_boneIndex);
	if (!nodeIndex.has_value())
	{
		return false;
	}

	OvMaths::FVector3 currentPosition;
	OvMaths::FQuaternion currentRotation;
	OvMaths::FVector3 currentScale;
	DecomposeLocalTransform(m_localPose[*nodeIndex], currentPosition, currentRotation, currentScale);

	const OvMaths::FTransform transform(currentPosition, p_rotation, currentScale);
	m_localPose[*nodeIndex] = transform.GetLocalMatrix();
	m_manualPoseOverride = true;
	RecomputeBoneMatricesFromLocalPose();
	return true;
}

bool OvCore::ECS::Components::CSkinnedMeshRenderer::SetBoneLocalScale(uint32_t p_boneIndex, const OvMaths::FVector3& p_scale)
{
	const auto nodeIndex = GetNodeIndexFromBoneIndex(p_boneIndex);
	if (!nodeIndex.has_value())
	{
		return false;
	}

	OvMaths::FVector3 currentPosition;
	OvMaths::FQuaternion currentRotation;
	OvMaths::FVector3 currentScale;
	DecomposeLocalTransform(m_localPose[*nodeIndex], currentPosition, currentRotation, currentScale);

	const OvMaths::FTransform transform(currentPosition, currentRotation, p_scale);
	m_localPose[*nodeIndex] = transform.GetLocalMatrix();
	m_manualPoseOverride = true;
	RecomputeBoneMatricesFromLocalPose();
	return true;
}

const std::vector<OvMaths::FMatrix4>& OvCore::ECS::Components::CSkinnedMeshRenderer::GetBoneMatricesTransposed() const
{
	return m_boneMatricesTransposed;
}

uint64_t OvCore::ECS::Components::CSkinnedMeshRenderer::GetPoseVersion() const
{
	return m_poseVersion;
}

void OvCore::ECS::Components::CSkinnedMeshRenderer::OnUpdate(float p_deltaTime)
{
	if (!owner.IsActive())
	{
		return;
	}

	SyncWithModel();

	if (!HasCompatibleModel())
	{
		return;
	}

	bool timeChanged = false;
	bool playbackStateChanged = false;

	for (auto& layer : m_layers)
	{
		if (!layer.playing)
		{
			continue;
		}

		const float previousTimeTicks = layer.timeTicks;

		UpdatePlayback(layer, p_deltaTime);

		timeChanged = timeChanged || std::abs(layer.timeTicks - previousTimeTicks) > std::numeric_limits<float>::epsilon();
		playbackStateChanged = playbackStateChanged || !layer.playing;
	}

	if (timeChanged || playbackStateChanged)
	{
		const float clampedPoseEvaluationRate = std::max(0.0f, m_poseEvaluationRate);
		const bool hasRateLimit = clampedPoseEvaluationRate > std::numeric_limits<float>::epsilon();

		if (hasRateLimit)
		{
			m_poseEvaluationAccumulator += p_deltaTime;
			const float updatePeriod = 1.0f / clampedPoseEvaluationRate;
			if (m_poseEvaluationAccumulator < updatePeriod && !playbackStateChanged)
			{
				return;
			}

			m_poseEvaluationAccumulator = std::fmod(m_poseEvaluationAccumulator, updatePeriod);
		}
		else
		{
			m_poseEvaluationAccumulator = 0.0f;
		}

		EvaluatePose();
	}
}

void OvCore::ECS::Components::CSkinnedMeshRenderer::OnSerialize(tinyxml2::XMLDocument& p_doc, tinyxml2::XMLNode* p_node)
{
	OvCore::Helpers::Serializer::SerializeFloat(p_doc, p_node, "mesh_bounds_scale", m_meshBoundsScale);
	OvCore::Helpers::Serializer::SerializeFloat(p_doc, p_node, "pose_eval_rate", m_poseEvaluationRate);

	tinyxml2::XMLNode* layersNode = p_doc.NewElement("layers");
	p_node->InsertEndChild(layersNode);

	for (uint32_t layerIndex = 0; layerIndex < m_layers.size(); ++layerIndex)
	{
		tinyxml2::XMLNode* layerNode = p_doc.NewElement("layer");
		layersNode->InsertEndChild(layerNode);

		const auto& layer = m_layers[layerIndex];
		OvCore::Helpers::Serializer::SerializeModel(p_doc, layerNode, "animation_source", layer.animationSourceModel);
		OvCore::Helpers::Serializer::SerializeString(p_doc, layerNode, "animation", GetActiveAnimationName(layerIndex).value_or(std::string{}));
		OvCore::Helpers::Serializer::SerializeFloat(p_doc, layerNode, "weight", layer.weight);
		OvCore::Helpers::Serializer::SerializeBoolean(p_doc, layerNode, "playing", layer.playing);
		OvCore::Helpers::Serializer::SerializeBoolean(p_doc, layerNode, "looping", layer.looping);
		OvCore::Helpers::Serializer::SerializeFloat(p_doc, layerNode, "playback_speed", layer.speed);
		OvCore::Helpers::Serializer::SerializeFloat(p_doc, layerNode, "time_ticks", layer.timeTicks);
	}
}

void OvCore::ECS::Components::CSkinnedMeshRenderer::OnDeserialize(tinyxml2::XMLDocument& p_doc, tinyxml2::XMLNode* p_node)
{
	OvCore::Helpers::Serializer::DeserializeFloat(p_doc, p_node, "mesh_bounds_scale", m_meshBoundsScale);
	OvCore::Helpers::Serializer::DeserializeFloat(p_doc, p_node, "pose_eval_rate", m_poseEvaluationRate);

	if (tinyxml2::XMLNode* layersRoot = p_node->FirstChildElement("layers"))
	{
		m_layers.clear();

		for (
			tinyxml2::XMLElement* currentLayer = layersRoot->FirstChildElement("layer");
			currentLayer;
			currentLayer = currentLayer->NextSiblingElement("layer")
		)
		{
			auto& layer = m_layers.emplace_back();
			OvCore::Helpers::Serializer::DeserializeModel(p_doc, currentLayer, "animation_source", layer.animationSourceModel);
			OvCore::Helpers::Serializer::DeserializeString(p_doc, currentLayer, "animation", layer.deserializedAnimationName);
			OvCore::Helpers::Serializer::DeserializeFloat(p_doc, currentLayer, "weight", layer.weight);
			OvCore::Helpers::Serializer::DeserializeBoolean(p_doc, currentLayer, "playing", layer.playing);
			OvCore::Helpers::Serializer::DeserializeBoolean(p_doc, currentLayer, "looping", layer.looping);
			OvCore::Helpers::Serializer::DeserializeFloat(p_doc, currentLayer, "playback_speed", layer.speed);
			OvCore::Helpers::Serializer::DeserializeFloat(p_doc, currentLayer, "time_ticks", layer.timeTicks);

			layer.weight = ClampLayerWeight(layer.weight);
		}

		// The base layer always exists, even when the scene holds an empty <layers> element
		if (m_layers.empty())
		{
			m_layers.emplace_back();
		}
	}

	SetMeshBoundsScale(m_meshBoundsScale);
	m_poseEvaluationRate = std::max(0.0f, m_poseEvaluationRate);
	m_poseEvaluationAccumulator = 0.0f;

	NotifyModelChanged();
}

void OvCore::ECS::Components::CSkinnedMeshRenderer::OnInspector(OvUI::Internal::WidgetContainer& p_root)
{
	SyncWithModel();

	using namespace OvCore::Helpers;

	GUIDrawer::DrawScalar<float>(p_root, "Mesh Bounds Scale", m_meshBoundsScale, 0.05f, 1.0f, 10.0f);
	GUIDrawer::DrawScalar<float>(p_root, "Pose Eval Rate", m_poseEvaluationRate, 1.0f, 0.0f, 240.0f);
	m_poseEvaluationRate = std::max(0.0f, m_poseEvaluationRate);

	auto& modelDiagnostic = p_root.CreateWidget<OvUI::Widgets::Texts::TextColored>();
	modelDiagnostic.AddPlugin<OvUI::Plugins::DataDispatcher<std::string>>().RegisterGatherer([this, &modelDiagnostic]
	{
		const bool ready = HasCompatibleModel();
		modelDiagnostic.color = ready ? OVUI_STYLE(Success) : OVUI_STYLE(TextDisabled);
		return ready ? std::string{ "Ready" } : std::string{ "No skinned model assigned" };
	});

	// Layers live in their own full-width container so they can be rebuilt in place when one is
	// added or removed, without disturbing the settings above
	auto& layersRoot = p_root.CreateWidget<OvUI::Widgets::Layout::Group>();
	layersRoot.fullWidth = true;
	BuildLayerWidgets(layersRoot);
}

void OvCore::ECS::Components::CSkinnedMeshRenderer::BuildLayerWidgets(OvUI::Internal::WidgetContainer& p_container)
{
	using namespace OvCore::Helpers;

	// Widgets are destroyed rather than removed, so this stays safe when called from the click
	// handler of one of the widgets being replaced
	for (auto& widget : p_container.GetWidgets())
	{
		widget.first->Destroy();
	}

	for (uint32_t layerIndex = 0; layerIndex < m_layers.size(); ++layerIndex)
	{
		// Layers are array elements, so they use a tree node rather than the collapsable group
		// reserved for components. The identifier keeps the node folded state across rebuilds
		auto& layerNode = p_container.CreateWidget<OvUI::Widgets::Layout::TreeNode>("Layer " + std::to_string(layerIndex));
		layerNode.SetID("skinned_layer_node_" + std::to_string(layerIndex));

		auto& columns = layerNode.CreateWidget<OvUI::Widgets::Layout::Columns<2>>();
		columns.SetID("skinned_layer_" + std::to_string(layerIndex));
		columns.widths[0] = 200 * OVUI_SCALE;

		// The animation source is bound through the setter, so the widget holds no reference into
		// the layer storage, which moves whenever a layer is added or removed
		GUIDrawer::DrawAsset(
			columns,
			"Animation Source",
			[this, layerIndex]
			{
				const auto model = GetAnimationSourceModel(layerIndex);
				return model ? model->path : std::string{};
			},
			[this, layerIndex](std::string p_path)
			{
				SetAnimationSourceModel(
					p_path.empty() ? nullptr : OVSERVICE(OvCore::ResourceManagement::ModelManager).GetResource(p_path),
					layerIndex
				);
			},
			OvTools::Utils::PathParser::EFileType::MODEL
		);

		GUIDrawer::CreateTitle(columns, "Animation");
		const auto activeAnimationIndex = GetActiveAnimationIndex(layerIndex);
		auto& animationChoice = columns.CreateWidget<OvUI::Widgets::Selection::ComboBox>(
			activeAnimationIndex.has_value() ? static_cast<int>(*activeAnimationIndex) : -1
		);

		auto& animDispatcher = animationChoice.AddPlugin<OvUI::Plugins::DataDispatcher<int>>();
		animDispatcher.RegisterGatherer([this, &animationChoice, layerIndex]
		{
			animationChoice.choices.clear();
			animationChoice.choices.emplace(-1, "<None>");

			for (uint32_t i = 0; i < GetAnimationCount(layerIndex); ++i)
			{
				animationChoice.choices.emplace(static_cast<int>(i), GetAnimationName(i, layerIndex).value_or(std::string{}));
			}

			const auto gatheredIndex = GetActiveAnimationIndex(layerIndex);
			return gatheredIndex.has_value() ? static_cast<int>(*gatheredIndex) : -1;
		});
		animDispatcher.RegisterProvider([this, layerIndex](int p_choice)
		{
			SetAnimation(p_choice >= 0 ? std::make_optional(static_cast<uint32_t>(p_choice)) : std::nullopt, layerIndex);
		});

		GUIDrawer::DrawScalar<float>(
			columns,
			"Weight",
			[this, layerIndex]() { return GetLayerWeight(layerIndex); },
			[this, layerIndex](float p_value) { SetLayerWeight(p_value, layerIndex); },
			0.01f,
			0.0f,
			1.0f
		);

		GUIDrawer::DrawBoolean(
			columns,
			"Playing",
			[this, layerIndex]() { return IsPlaying(layerIndex); },
			[this, layerIndex](bool p_value)
			{
				if (p_value)
				{
					Play(layerIndex);
				}
				else
				{
					Pause(layerIndex);
				}
			}
		);

		GUIDrawer::DrawBoolean(
			columns,
			"Looping",
			[this, layerIndex]() { return IsLooping(layerIndex); },
			[this, layerIndex](bool p_value) { SetLooping(p_value, layerIndex); }
		);

		GUIDrawer::DrawScalar<float>(
			columns,
			"Playback Speed",
			[this, layerIndex]() { return GetPlaybackSpeed(layerIndex); },
			[this, layerIndex](float p_value) { SetPlaybackSpeed(p_value, layerIndex); },
			0.01f,
			-10.0f,
			10.0f
		);

		GUIDrawer::DrawScalar<float>(
			columns,
			"Time (Seconds)",
			[this, layerIndex]() { return GetTime(layerIndex); },
			[this, layerIndex](float p_value) { SetTime(p_value, layerIndex); },
			0.01f,
			0.0f,
			std::max(GetAnimationDurationSeconds(layerIndex), 3600.0f)
		);

		// Gathered every frame, so assigning an incompatible source reports it without a panel refresh
		auto& diagnostic = layerNode.CreateWidget<OvUI::Widgets::Texts::TextColored>();
		diagnostic.AddPlugin<OvUI::Plugins::DataDispatcher<std::string>>().RegisterGatherer([this, &diagnostic, layerIndex]
		{
			const auto layer = FindLayer(layerIndex);
			if (!layer)
			{
				return std::string{};
			}

			if (layer->animationSourceModel && !IsLayerCompatible(*layer))
			{
				diagnostic.color = OVUI_STYLE(Danger);
				return std::string{ "Animation source skeleton is not compatible with model" };
			}

			if (layer->animationNames.empty())
			{
				diagnostic.color = OVUI_STYLE(Warning);
				return std::string{ layer->animationSourceModel ? "Animation source has no animation clips" : "Model has no animation clips" };
			}

			return std::string{};
		});

		if (m_layers.size() > 1)
		{
			auto& removeLayerButton = layerNode.CreateWidget<OvUI::Widgets::Buttons::Button>("Remove Layer");
			removeLayerButton.ClickedEvent += [this, &p_container, layerIndex]
			{
				RemoveLayer(layerIndex);
				BuildLayerWidgets(p_container);
			};
		}
	}

	auto& addLayerButton = p_container.CreateWidget<OvUI::Widgets::Buttons::Button>("Add Layer");
	addLayerButton.ClickedEvent += [this, &p_container]
	{
		AddLayer();
		BuildLayerWidgets(p_container);
	};
}

bool OvCore::ECS::Components::CSkinnedMeshRenderer::HasCompatibleModel() const
{
	return m_model && m_model->IsSkinned() && m_model->GetSkeleton().has_value();
}

bool OvCore::ECS::Components::CSkinnedMeshRenderer::IsLayerCompatible(const AnimationLayer& p_layer) const
{
	return HasCompatibleModel() && !p_layer.sourceNodeByTargetNode.empty();
}

const OvRendering::Resources::Model* OvCore::ECS::Components::CSkinnedMeshRenderer::GetLayerAnimationModel(const AnimationLayer& p_layer) const
{
	return p_layer.animationSourceModel ? p_layer.animationSourceModel : m_model;
}

OvCore::ECS::Components::CSkinnedMeshRenderer::AnimationLayer* OvCore::ECS::Components::CSkinnedMeshRenderer::FindLayer(uint32_t p_layer)
{
	return p_layer < m_layers.size() ? &m_layers[p_layer] : nullptr;
}

const OvCore::ECS::Components::CSkinnedMeshRenderer::AnimationLayer* OvCore::ECS::Components::CSkinnedMeshRenderer::FindLayer(uint32_t p_layer) const
{
	return p_layer < m_layers.size() ? &m_layers[p_layer] : nullptr;
}

void OvCore::ECS::Components::CSkinnedMeshRenderer::SyncWithModel()
{
	const auto modelRenderer = owner.GetComponent<CModelRenderer>();
	const auto model = modelRenderer ? modelRenderer->GetModel() : nullptr;

	if (m_model == model)
	{
		return;
	}

	m_model = model;
	RebuildRuntimeData();
}

void OvCore::ECS::Components::CSkinnedMeshRenderer::RebuildRuntimeData()
{
	m_localPose.clear();
	m_globalPose.clear();
	m_boneMatrices.clear();
	m_boneMatricesTransposed.clear();
	m_poseEvaluationAccumulator = 0.0f;
	m_manualPoseOverride = false;

	for (auto& layer : m_layers)
	{
		layer.animationNames.clear();
		layer.sourceNodeByTargetNode.clear();
	}

	if (!HasCompatibleModel())
	{
		// Pending deserialized names are kept, so they can still be resolved once a model shows up
		for (auto& layer : m_layers)
		{
			layer.animationIndex = std::nullopt;
		}

		return;
	}

	const auto& skeleton = m_model->GetSkeleton().value();

	m_localPose.resize(skeleton.nodes.size(), OvMaths::FMatrix4::Identity);
	m_globalPose.resize(skeleton.nodes.size(), OvMaths::FMatrix4::Identity);
	m_boneMatrices.resize(skeleton.bones.size(), OvMaths::FMatrix4::Identity);
	m_boneMatricesTransposed.resize(skeleton.bones.size(), OvMaths::FMatrix4::Identity);

	std::vector<int32_t> nodeMapScratch;

	for (auto& layer : m_layers)
	{
		RebuildLayerRuntimeData(layer, nodeMapScratch);
		ResolveLayerAnimation(layer);
	}

	EvaluatePose();
}

void OvCore::ECS::Components::CSkinnedMeshRenderer::RebuildLayerRuntimeData(AnimationLayer& p_layer, std::vector<int32_t>& p_nodeMapScratch)
{
	const auto& skeleton = m_model->GetSkeleton().value();
	const auto animationModel = GetLayerAnimationModel(p_layer);

	if (
		!animationModel ||
		!animationModel->GetSkeleton().has_value() ||
		!BuildAnimationNodeMap(skeleton, animationModel->GetSkeleton().value(), p_nodeMapScratch)
	)
	{
		if (p_layer.animationSourceModel)
		{
			OVLOG_WARNING("SkinnedMeshRenderer: Animation source model '" + p_layer.animationSourceModel->path + "' is not compatible with target model '" + m_model->path + "'.");
		}

		return;
	}

	p_layer.sourceNodeByTargetNode.assign(skeleton.nodes.size(), -1);
	for (size_t sourceNodeIndex = 0; sourceNodeIndex < p_nodeMapScratch.size(); ++sourceNodeIndex)
	{
		const int32_t targetNodeIndex = p_nodeMapScratch[sourceNodeIndex];
		if (targetNodeIndex >= 0 && static_cast<size_t>(targetNodeIndex) < p_layer.sourceNodeByTargetNode.size())
		{
			p_layer.sourceNodeByTargetNode[targetNodeIndex] = static_cast<int32_t>(sourceNodeIndex);
		}
	}

	const auto& animations = animationModel->GetAnimations();

	p_layer.animationNames.reserve(animations.size());
	for (const auto& animation : animations)
	{
		p_layer.animationNames.push_back(animation.name);
	}
}

void OvCore::ECS::Components::CSkinnedMeshRenderer::ResolveLayerAnimation(AnimationLayer& p_layer)
{
	const std::optional<uint32_t> preservedAnimationIndex = p_layer.animationIndex;
	const float preservedTimeTicks = p_layer.timeTicks;
	const std::string requestedAnimationName = p_layer.deserializedAnimationName;

	p_layer.animationIndex = std::nullopt;
	p_layer.deserializedAnimationName.clear();

	const auto& animationNames = p_layer.animationNames;

	if (!animationNames.empty())
	{
		if (!requestedAnimationName.empty())
		{
			const auto found = std::find(animationNames.begin(), animationNames.end(), requestedAnimationName);
			p_layer.animationIndex = found != animationNames.end() ?
				std::optional<uint32_t>{ static_cast<uint32_t>(std::distance(animationNames.begin(), found)) } :
				std::optional<uint32_t>{ 0 };
		}
		else if (preservedAnimationIndex.has_value() && *preservedAnimationIndex < animationNames.size())
		{
			p_layer.animationIndex = *preservedAnimationIndex;
		}
	}

	const auto& animations = GetLayerAnimationModel(p_layer)->GetAnimations();

	if (IsLayerCompatible(p_layer) && p_layer.animationIndex.has_value() && *p_layer.animationIndex < animations.size())
	{
		const auto& animation = animations.at(*p_layer.animationIndex);
		p_layer.timeTicks = p_layer.looping ?
			WrapTime(preservedTimeTicks, animation.duration) :
			std::clamp(preservedTimeTicks, 0.0f, animation.duration);
	}
	else
	{
		p_layer.timeTicks = 0.0f;
	}
}

void OvCore::ECS::Components::CSkinnedMeshRenderer::EvaluatePose()
{
	if (!HasCompatibleModel())
	{
		return;
	}

	const auto& skeleton = m_model->GetSkeleton().value();

	m_activeLayerSamples.clear();

	for (const auto& layer : m_layers)
	{
		if (layer.sourceNodeByTargetNode.size() != skeleton.nodes.size())
		{
			continue;
		}

		if (!layer.animationIndex.has_value() || !(layer.weight > kMinimumLayerWeight))
		{
			continue;
		}

		const auto& animations = GetLayerAnimationModel(layer)->GetAnimations();
		if (*layer.animationIndex >= animations.size())
		{
			continue;
		}

		const auto& animation = animations.at(*layer.animationIndex);
		const float duration = std::max(animation.duration, 0.0f);
		const float sampleTime =
			duration > 0.0f ?
			(layer.looping ? WrapTime(layer.timeTicks, duration) : std::clamp(layer.timeTicks, 0.0f, duration)) :
			0.0f;

		m_activeLayerSamples.push_back({ &animation, sampleTime, duration, layer.weight, layer.looping, &layer.sourceNodeByTargetNode });
	}

	for (size_t targetNodeIndex = 0; targetNodeIndex < skeleton.nodes.size(); ++targetNodeIndex)
	{
		const auto& node = skeleton.nodes[targetNodeIndex];

		OvMaths::FVector3 blendedPosition;
		OvMaths::FQuaternion blendedRotation;
		OvMaths::FVector3 blendedScale;
		float accumulatedWeight = 0.0f;

		for (const auto& activeLayer : m_activeLayerSamples)
		{
			const int32_t sourceNodeIndex = (*activeLayer.sourceNodeByTargetNode)[targetNodeIndex];
			if (sourceNodeIndex < 0)
			{
				continue;
			}

			const auto track = activeLayer.animation->FindTrack(static_cast<uint32_t>(sourceNodeIndex));
			if (!track)
			{
				continue;
			}

			const OvMaths::FVector3 sampledPosition = SampleKeys(
				track->positionKeys,
				activeLayer.sampleTime,
				activeLayer.duration,
				node.bindPosition,
				activeLayer.looping,
				[](const auto& p_a, const auto& p_b, float p_alpha) { return OvMaths::FVector3::Lerp(p_a, p_b, p_alpha); }
			);

			const OvMaths::FQuaternion sampledRotation = SampleKeys(
				track->rotationKeys,
				activeLayer.sampleTime,
				activeLayer.duration,
				node.bindRotation,
				activeLayer.looping,
				[](const auto& p_a, const auto& p_b, float p_alpha) { return OvMaths::FQuaternion::Slerp(p_a, p_b, p_alpha); }
			);

			const OvMaths::FVector3 sampledScale = SampleKeys(
				track->scaleKeys,
				activeLayer.sampleTime,
				activeLayer.duration,
				node.bindScale,
				activeLayer.looping,
				[](const auto& p_a, const auto& p_b, float p_alpha) { return OvMaths::FVector3::Lerp(p_a, p_b, p_alpha); }
			);

			if (accumulatedWeight <= 0.0f)
			{
				blendedPosition = sampledPosition;
				blendedRotation = sampledRotation;
				blendedScale = sampledScale;
			}
			else
			{
				const float alpha = activeLayer.weight / (accumulatedWeight + activeLayer.weight);
				blendedPosition = OvMaths::FVector3::Lerp(blendedPosition, sampledPosition, alpha);
				blendedRotation = OvMaths::FQuaternion::Slerp(blendedRotation, sampledRotation, alpha);
				blendedScale = OvMaths::FVector3::Lerp(blendedScale, sampledScale, alpha);
			}

			accumulatedWeight += activeLayer.weight;
		}

		if (accumulatedWeight > 0.0f)
		{
			const OvMaths::FTransform blended(blendedPosition, blendedRotation, blendedScale);
			m_localPose[targetNodeIndex] = blended.GetLocalMatrix();
		}
		else
		{
			m_localPose[targetNodeIndex] = node.localBindTransform;
		}
	}

	m_manualPoseOverride = false;
	RecomputeBoneMatricesFromLocalPose();
}

std::optional<uint32_t> OvCore::ECS::Components::CSkinnedMeshRenderer::GetNodeIndexFromBoneIndex(uint32_t p_boneIndex) const
{
	if (!HasCompatibleModel())
	{
		return std::nullopt;
	}

	const auto& skeleton = m_model->GetSkeleton().value();
	if (p_boneIndex >= skeleton.bones.size())
	{
		return std::nullopt;
	}

	const uint32_t nodeIndex = skeleton.bones[p_boneIndex].nodeIndex;
	if (nodeIndex >= m_localPose.size())
	{
		return std::nullopt;
	}

	return nodeIndex;
}

void OvCore::ECS::Components::CSkinnedMeshRenderer::RecomputeBoneMatricesFromLocalPose()
{
	if (!HasCompatibleModel())
	{
		return;
	}

	const auto& skeleton = m_model->GetSkeleton().value();
	if (
		m_localPose.size() != skeleton.nodes.size() ||
		m_globalPose.size() != skeleton.nodes.size() ||
		m_boneMatrices.size() != skeleton.bones.size() ||
		m_boneMatricesTransposed.size() != skeleton.bones.size()
	)
	{
		return;
	}

	for (size_t nodeIndex = 0; nodeIndex < skeleton.nodes.size(); ++nodeIndex)
	{
		const auto parentIndex = skeleton.nodes[nodeIndex].parentIndex;
		m_globalPose[nodeIndex] =
			parentIndex >= 0 ?
			m_globalPose[static_cast<size_t>(parentIndex)] * m_localPose[nodeIndex] :
			m_localPose[nodeIndex];
	}

	for (size_t boneIndex = 0; boneIndex < skeleton.bones.size(); ++boneIndex)
	{
		const auto& bone = skeleton.bones[boneIndex];
		m_boneMatrices[boneIndex] =
			bone.nodeIndex < m_globalPose.size() ?
			m_globalPose[bone.nodeIndex] * bone.offsetMatrix :
			OvMaths::FMatrix4::Identity;

		m_boneMatricesTransposed[boneIndex] = OvMaths::FMatrix4::Transpose(m_boneMatrices[boneIndex]);
	}

	++m_poseVersion;
}

float OvCore::ECS::Components::CSkinnedMeshRenderer::GetAnimationDurationSeconds(uint32_t p_layer) const
{
	const auto layer = FindLayer(p_layer);
	if (!layer || !IsLayerCompatible(*layer) || !layer->animationIndex.has_value())
	{
		return 0.0f;
	}

	const auto& animation = GetLayerAnimationModel(*layer)->GetAnimations().at(*layer->animationIndex);
	return animation.GetDurationSeconds();
}

void OvCore::ECS::Components::CSkinnedMeshRenderer::UpdatePlayback(AnimationLayer& p_layer, float p_deltaTime)
{
	if (!IsLayerCompatible(p_layer) || !p_layer.animationIndex.has_value())
	{
		return;
	}

	const auto& animations = GetLayerAnimationModel(p_layer)->GetAnimations();
	if (*p_layer.animationIndex >= animations.size())
	{
		return;
	}

	const auto& animation = animations.at(*p_layer.animationIndex);
	if (animation.duration <= 0.0f)
	{
		return;
	}

	if (std::abs(p_layer.speed) <= std::numeric_limits<float>::epsilon())
	{
		return;
	}

	const float ticksPerSecond = animation.GetEffectiveTicksPerSecond();
	p_layer.timeTicks += p_deltaTime * ticksPerSecond * p_layer.speed;

	if (p_layer.looping)
	{
		p_layer.timeTicks = WrapTime(p_layer.timeTicks, animation.duration);
	}
	else
	{
		const float clamped = std::clamp(p_layer.timeTicks, 0.0f, animation.duration);
		const bool reachedStart = clamped <= 0.0f && p_layer.speed < 0.0f;
		const bool reachedEnd = clamped >= animation.duration && p_layer.speed > 0.0f;
		p_layer.timeTicks = clamped;
		if (reachedStart || reachedEnd)
		{
			p_layer.playing = false;
		}
	}
}
