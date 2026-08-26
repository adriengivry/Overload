/**
* @project: Overload
* @author: Overload Tech.
* @licence: MIT
*/

#pragma once

#include <cstddef>
#include <optional>
#include <unordered_map>

#include <OvMaths/FMatrix4.h>
#include <OvMaths/FQuaternion.h>
#include <OvMaths/FVector2.h>
#include <OvMaths/FVector3.h>

namespace OvCore::ECS { class Actor; }
namespace OvCore::ECS::Components::UI { class CCanvas; }

namespace OvCore::Rendering::UIRenderingUtils
{
	struct ResolvedUICanvas
	{
		const OvCore::ECS::Actor* actor = nullptr;
		const OvCore::ECS::Components::UI::CCanvas* canvas = nullptr;
		OvMaths::FVector2 size = OvMaths::FVector2::Zero;
		OvMaths::FMatrix4 matrix = OvMaths::FMatrix4::Identity;
		OvMaths::FMatrix4 modelMatrix = OvMaths::FMatrix4::Identity;
		float canvasScale = 1.0f;
		float worldScale = 1.0f;
		float unitsScale = 1.0f;
		bool screenSpace = false;
	};

	struct ResolvedUIElement
	{
		const OvCore::ECS::Actor* actor = nullptr;
		const OvCore::ECS::Actor* canvasActor = nullptr;
		const OvCore::ECS::Components::UI::CCanvas* canvas = nullptr;
		OvMaths::FVector2 canvasSize = OvMaths::FVector2::Zero;
		OvMaths::FVector2 layoutOffset = OvMaths::FVector2::Zero;
		OvMaths::FVector2 elementSize = OvMaths::FVector2::Zero;
		OvMaths::FVector2 effectiveSize = OvMaths::FVector2::Zero;
		OvMaths::FMatrix4 canvasMatrix = OvMaths::FMatrix4::Identity;
		OvMaths::FMatrix4 localMatrix = OvMaths::FMatrix4::Identity;
		OvMaths::FMatrix4 frameMatrix = OvMaths::FMatrix4::Identity;
		OvMaths::FMatrix4 modelMatrix = OvMaths::FMatrix4::Identity;
		OvMaths::FVector2 xPositionDirection = { 1.0f, 0.0f };
		OvMaths::FVector2 yPositionDirection = { 0.0f, 1.0f };
		OvMaths::FVector3 xWorldAxis = OvMaths::FVector3::Right;
		OvMaths::FVector3 yWorldAxis = OvMaths::FVector3::Up;
		float canvasScale = 1.0f;
		float worldScale = 1.0f;
		float unitsScale = 1.0f;
		bool widthDriven = false;
		bool heightDriven = false;
		bool screenSpace = false;
	};

	struct ResolvedUIGizmoTransform
	{
		OvMaths::FVector3 position = OvMaths::FVector3::Zero;
		OvMaths::FQuaternion rotation = OvMaths::FQuaternion::Identity;
		OvMaths::FVector2 xPositionDirection = { 1.0f, 0.0f };
		OvMaths::FVector2 yPositionDirection = { 0.0f, 1.0f };
		OvMaths::FVector3 xWorldAxis = OvMaths::FVector3::Right;
		OvMaths::FVector3 yWorldAxis = OvMaths::FVector3::Up;
		bool screenSpace = false;
	};

	class UIFrameResolver
	{
	public:
		UIFrameResolver(
			const OvMaths::FVector2& p_renderSize = { 1.0f, 1.0f },
			bool p_screenSpace = true
		);

		const OvMaths::FVector2& GetRenderSize() const;
		bool IsScreenSpace() const;

		OvMaths::FMatrix4 CreateProjectionMatrix(
			float p_near = -1.0f,
			float p_far = 1.0f
		) const;

		OvMaths::FVector2 GetElementSize(const OvCore::ECS::Actor& p_actor) const;

		bool ResolveCanvas(
			const OvCore::ECS::Actor& p_actor,
			ResolvedUICanvas& p_outCanvas
		) const;

		bool ResolveElement(
			const OvCore::ECS::Actor& p_actor,
			const OvMaths::FVector2& p_elementSize,
			ResolvedUIElement& p_outElement
		) const;

		bool ResolveElement(
			const OvCore::ECS::Actor& p_actor,
			ResolvedUIElement& p_outElement
		) const;

	private:
		struct ElementKey
		{
			const OvCore::ECS::Actor* actor = nullptr;
			float width = 0.0f;
			float height = 0.0f;

			bool operator==(const ElementKey& p_other) const;
		};

		struct ElementKeyHash
		{
			std::size_t operator()(const ElementKey& p_key) const;
		};

		struct CachedLayoutData
		{
			OvMaths::FVector2 offset = OvMaths::FVector2::Zero;
			OvMaths::FVector2 directSize = OvMaths::FVector2::Zero;
			bool hasDirectWidth = false;
			bool hasDirectHeight = false;
			bool drivenByLayout = false;
		};

		bool ResolveCanvasUncached(
			const OvCore::ECS::Actor& p_actor,
			ResolvedUICanvas& p_outCanvas
		) const;

		bool ResolveElementUncached(
			const OvCore::ECS::Actor& p_actor,
			const OvMaths::FVector2& p_elementSize,
			ResolvedUIElement& p_outElement
		) const;

		bool HasActiveUIData(const OvCore::ECS::Actor& p_actor) const;
		const OvCore::ECS::Actor* FindCanvasOwner(const OvCore::ECS::Actor& p_actor) const;
		CachedLayoutData GetLayoutData(
			const OvCore::ECS::Actor& p_actor,
			const OvMaths::FVector2& p_parentSize
		) const;

	private:
		OvMaths::FVector2 m_renderSize = { 1.0f, 1.0f };
		bool m_screenSpace = true;
		mutable std::unordered_map<const OvCore::ECS::Actor*, bool> m_activeUIDataCache;
		mutable std::unordered_map<const OvCore::ECS::Actor*, const OvCore::ECS::Actor*> m_canvasOwnerCache;
		mutable std::unordered_map<ElementKey, CachedLayoutData, ElementKeyHash> m_layoutDataCache;
		mutable std::unordered_map<const OvCore::ECS::Actor*, OvMaths::FVector2> m_elementSizeCache;
		mutable std::unordered_map<const OvCore::ECS::Actor*, std::optional<ResolvedUICanvas>> m_canvasCache;
		mutable std::unordered_map<ElementKey, std::optional<ResolvedUIElement>, ElementKeyHash> m_elementCache;
	};

	OvMaths::FVector2 ClampCanvasSize(const OvMaths::FVector2& p_canvasSize);

	OvMaths::FMatrix4 CreateUIProjectionMatrix(
		const OvMaths::FVector2& p_renderSize,
		float p_near = -1.0f,
		float p_far = 1.0f
	);

	OvMaths::FVector2 GetCanvasSize(
		const OvCore::ECS::Components::UI::CCanvas& p_canvas,
		const OvMaths::FVector2& p_renderSize
	);

	float GetCanvasScale(
		const OvCore::ECS::Components::UI::CCanvas& p_canvas,
		const OvMaths::FVector2& p_renderSize
	);

	const OvCore::ECS::Components::UI::CCanvas* FindCanvas(const OvCore::ECS::Actor& p_owner);

	OvCore::ECS::Actor* FindCanvasOwner(OvCore::ECS::Actor& p_owner);
	const OvCore::ECS::Actor* FindCanvasOwner(const OvCore::ECS::Actor& p_owner);

	OvMaths::FVector2 GetCanvasSize(
		const OvCore::ECS::Actor& p_owner,
		const OvMaths::FVector2& p_renderSize
	);

	OvMaths::FVector2 GetElementSize(
		const OvCore::ECS::Actor& p_owner,
		const OvMaths::FVector2& p_renderSize
	);

	float GetUIWorldScale(bool p_screenSpace);

	OvMaths::FVector3 TransformUIPoint(
		const OvMaths::FMatrix4& p_matrix,
		const OvMaths::FVector2& p_point
	);

	OvMaths::FVector3 TransformUIElementPivot(const ResolvedUIElement& p_element);

	bool ResolveUIGizmoTransform(
		const UIFrameResolver& p_frameResolver,
		const OvCore::ECS::Actor& p_actor,
		ResolvedUIGizmoTransform& p_outTransform
	);

}
