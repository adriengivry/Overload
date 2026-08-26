/**
* @project: Overload
* @author: Overload Tech.
* @licence: MIT
*/

#include <algorithm>
#include <cmath>
#include <cstddef>
#include <cstdint>
#include <limits>
#include <memory>
#include <mutex>
#include <string>
#include <vector>

#ifdef _MSC_VER
#pragma warning(push)
#pragma warning(disable: 4244 4305)
#endif

#include <clay.h>

#ifdef _MSC_VER
#pragma warning(pop)
#endif

#include <OvCore/ECS/Actor.h>
#include <OvCore/ECS/Components/UI/ClayLayoutSolver.h>

namespace
{
	constexpr float kMinimumLayoutSize = 1.0f;
	constexpr float kMaximumLayoutSize = 1'000'000.0f;
	constexpr float kMinimumSpacing = 0.0f;
	constexpr float kMinimumPadding = 0.0f;
	constexpr float kMaximumSpacing = static_cast<float>(std::numeric_limits<uint16_t>::max());
	constexpr float kMaximumPadding = static_cast<float>(std::numeric_limits<uint16_t>::max());
	constexpr int32_t kMinimumElementCapacity = 64;
	constexpr int32_t kSolverElementOverhead = 8;

	std::mutex& GetClayApiMutex()
	{
		// Clay stores its current context globally, so isolate that mutable API behind one lock.
		static std::mutex mutex;
		return mutex;
	}

	struct ClayLayoutPassResult
	{
		OvMaths::FVector2 size = OvMaths::FVector2::Zero;
		std::vector<Clay_BoundingBox> slotBoxes;
		std::vector<bool> slotFound;
	};

	class ScopedClayContext
	{
	public:
		explicit ScopedClayContext(Clay_Context* p_context) :
		m_previousContext(Clay_GetCurrentContext())
		{
			Clay_SetCurrentContext(p_context);
		}

		~ScopedClayContext()
		{
			Clay_SetCurrentContext(m_previousContext);
		}

	private:
		Clay_Context* m_previousContext = nullptr;
	};

	class ClaySolverRuntime
	{
	public:
		Clay_Context* GetContext(size_t p_childCount)
		{
			constexpr auto maxChildCount = static_cast<size_t>(std::numeric_limits<int32_t>::max() - kSolverElementOverhead);
			if (p_childCount > maxChildCount)
			{
				return nullptr;
			}

			const auto requiredCapacity = std::max(
				kMinimumElementCapacity,
				static_cast<int32_t>(p_childCount) + kSolverElementOverhead
			);

			if (!m_context || requiredCapacity > m_elementCapacity)
			{
				Initialize(requiredCapacity);
			}

			return m_context;
		}

	private:
		static void HandleError(Clay_ErrorData p_errorData)
		{
			auto* runtime = static_cast<ClaySolverRuntime*>(p_errorData.userData);
			if (!runtime)
			{
				return;
			}

			runtime->m_lastError.assign(p_errorData.errorText.chars, p_errorData.errorText.length);
		}

		void Initialize(int32_t p_elementCapacity)
		{
			std::lock_guard lock(GetClayApiMutex());
			auto* previousContext = Clay_GetCurrentContext();

			Clay_SetCurrentContext(nullptr);
			Clay_SetMaxElementCount(p_elementCapacity);

			const auto memorySize = Clay_MinMemorySize();
			m_memory.resize(memorySize);
			auto arena = Clay_CreateArenaWithCapacityAndMemory(m_memory.size(), m_memory.data());
			m_context = Clay_Initialize(
				arena,
				{ kMaximumLayoutSize, kMaximumLayoutSize },
				{ &ClaySolverRuntime::HandleError, this }
			);

			m_elementCapacity = p_elementCapacity;

			if (m_context)
			{
				ScopedClayContext useContext(m_context);
				Clay_SetCullingEnabled(false);
			}

			Clay_SetCurrentContext(previousContext);
		}

	private:
		Clay_Context* m_context = nullptr;
		int32_t m_elementCapacity = 0;
		std::vector<char> m_memory;
		std::string m_lastError;
	};

	float KeepFinite(float p_value, float p_fallback)
	{
		return std::isfinite(p_value) ? p_value : p_fallback;
	}

	float ClampNonNegative(float p_value)
	{
		return std::max(KeepFinite(p_value, 0.0f), 0.0f);
	}

	uint16_t ToClaySpacing(float p_spacing)
	{
		return static_cast<uint16_t>(std::clamp(KeepFinite(p_spacing, kMinimumSpacing), kMinimumSpacing, kMaximumSpacing));
	}

	Clay_Padding ToClayPadding(const OvMaths::FVector4& p_padding)
	{
		return {
			.left = static_cast<uint16_t>(std::clamp(KeepFinite(p_padding.x, kMinimumPadding), kMinimumPadding, kMaximumPadding)),
			.right = static_cast<uint16_t>(std::clamp(KeepFinite(p_padding.y, kMinimumPadding), kMinimumPadding, kMaximumPadding)),
			.top = static_cast<uint16_t>(std::clamp(KeepFinite(p_padding.z, kMinimumPadding), kMinimumPadding, kMaximumPadding)),
			.bottom = static_cast<uint16_t>(std::clamp(KeepFinite(p_padding.w, kMinimumPadding), kMinimumPadding, kMaximumPadding))
		};
	}

	Clay_LayoutDirection ToClayDirection(OvCore::ECS::Components::UI::CLayoutGroup::EDirection p_direction)
	{
		return p_direction == OvCore::ECS::Components::UI::CLayoutGroup::EDirection::VERTICAL ?
			CLAY_TOP_TO_BOTTOM :
			CLAY_LEFT_TO_RIGHT;
	}

	Clay_SizingAxis MakeFixedSizing(float p_size)
	{
		return CLAY_SIZING_FIXED(ClampNonNegative(p_size));
	}

	Clay_SizingAxis MakeFitSizing(float p_minSize)
	{
		return CLAY_SIZING_FIT(std::max(ClampNonNegative(p_minSize), kMinimumLayoutSize), kMaximumLayoutSize);
	}

	Clay_SizingAxis MakeGrowSizing(float p_minSize)
	{
		return CLAY_SIZING_GROW(ClampNonNegative(p_minSize), kMaximumLayoutSize);
	}

	Clay_SizingAxis MakeSlotSizing(float p_preferredSize, bool p_forceExpand)
	{
		return p_forceExpand ? MakeGrowSizing(p_preferredSize) : MakeFixedSizing(p_preferredSize);
	}

	Clay_ElementDeclaration CreateContainerDeclaration(
		const OvCore::ECS::Components::UI::ClayLayoutSettings& p_settings,
		Clay_SizingAxis p_widthSizing,
		Clay_SizingAxis p_heightSizing
	)
	{
		Clay_ElementDeclaration declaration{};
		declaration.layout.sizing.width = p_widthSizing;
		declaration.layout.sizing.height = p_heightSizing;
		declaration.layout.padding = ToClayPadding(p_settings.padding);
		declaration.layout.childGap = ToClaySpacing(p_settings.spacing);
		declaration.layout.layoutDirection = ToClayDirection(p_settings.direction);
		return declaration;
	}

	Clay_ElementDeclaration CreateSlotDeclaration(
		const OvCore::ECS::Components::UI::ClayLayoutSettings& p_settings,
		const OvCore::ECS::Components::UI::ClayLayoutChildInput& p_child,
		bool p_resolveFlexibleSizing
	)
	{
		Clay_ElementDeclaration declaration{};
		declaration.layout.sizing.width = p_resolveFlexibleSizing ?
			MakeSlotSizing(p_child.preferredSize.x, p_settings.forceExpandWidth) :
			MakeFixedSizing(p_child.preferredSize.x);
		declaration.layout.sizing.height = p_resolveFlexibleSizing ?
			MakeSlotSizing(p_child.preferredSize.y, p_settings.forceExpandHeight) :
			MakeFixedSizing(p_child.preferredSize.y);
		return declaration;
	}

	ClayLayoutPassResult RunClayPass(
		Clay_Context* p_context,
		const OvCore::ECS::Components::UI::ClayLayoutSettings& p_settings,
		const std::vector<OvCore::ECS::Components::UI::ClayLayoutChildInput>& p_children,
		Clay_SizingAxis p_containerWidthSizing,
		Clay_SizingAxis p_containerHeightSizing,
		const OvMaths::FVector2& p_rootSize,
		bool p_resolveFlexibleSlots
	)
	{
		std::lock_guard lock(GetClayApiMutex());
		ScopedClayContext useContext(p_context);

		Clay_SetLayoutDimensions({
			std::max(ClampNonNegative(p_rootSize.x), kMinimumLayoutSize),
			std::max(ClampNonNegative(p_rootSize.y), kMinimumLayoutSize)
		});

		const auto containerId = CLAY_ID("Overload_LayoutGroup_Container");
		const auto slotIdBase = CLAY_STRING("Overload_LayoutGroup_Slot");
		std::vector<Clay_ElementId> slotIds;
		slotIds.reserve(p_children.size());

		Clay_BeginLayout();

		CLAY(containerId, CreateContainerDeclaration(p_settings, p_containerWidthSizing, p_containerHeightSizing))
		{
			for (size_t i = 0; i < p_children.size(); ++i)
			{
				const auto slotId = Clay_GetElementIdWithIndex(slotIdBase, static_cast<uint32_t>(i));
				slotIds.push_back(slotId);
				CLAY(slotId, CreateSlotDeclaration(p_settings, p_children[i], p_resolveFlexibleSlots))
				{
				}
			}
		}

		Clay_EndLayout(0.0f);

		ClayLayoutPassResult result;

		const auto containerData = Clay_GetElementData(containerId);
		if (!containerData.found)
		{
			return result;
		}

		result.size = {
			std::max(containerData.boundingBox.width, kMinimumLayoutSize),
			std::max(containerData.boundingBox.height, kMinimumLayoutSize)
		};

		result.slotBoxes.reserve(slotIds.size());
		result.slotFound.reserve(slotIds.size());
		for (const auto& slotId : slotIds)
		{
			const auto slotData = Clay_GetElementData(slotId);
			result.slotBoxes.push_back(slotData.found ? slotData.boundingBox : Clay_BoundingBox{});
			result.slotFound.push_back(slotData.found);
		}

		return result;
	}

	OvMaths::FVector2 GetRootSize(const OvMaths::FVector2& p_containerSize, const OvMaths::FVector2& p_preferredSize)
	{
		return {
			p_containerSize.x > 0.0f ? p_containerSize.x : std::max(p_preferredSize.x, kMinimumLayoutSize),
			p_containerSize.y > 0.0f ? p_containerSize.y : std::max(p_preferredSize.y, kMinimumLayoutSize)
		};
	}

	OvMaths::FVector2 ToChildOffset(
		const OvMaths::FVector2& p_childTopLeft,
		const OvMaths::FVector2& p_childSize,
		const OvMaths::FVector2& p_layoutSize
	)
	{
		const float centerX = p_childTopLeft.x + p_childSize.x * 0.5f;
		const float centerY = p_childTopLeft.y + p_childSize.y * 0.5f;
		const auto halfSize = p_layoutSize * 0.5f;

		return {
			centerX - halfSize.x,
			halfSize.y - centerY
		};
	}

	float GetHorizontalAlignmentFactor(
		OvCore::ECS::Components::UI::CLayoutGroup::EHorizontalAlignment p_alignment
	)
	{
		switch (p_alignment)
		{
		case OvCore::ECS::Components::UI::CLayoutGroup::EHorizontalAlignment::LEFT:
			return 0.0f;
		case OvCore::ECS::Components::UI::CLayoutGroup::EHorizontalAlignment::RIGHT:
			return 1.0f;
		case OvCore::ECS::Components::UI::CLayoutGroup::EHorizontalAlignment::CENTER:
		default:
			return 0.5f;
		}
	}

	float GetVerticalAlignmentFactor(
		OvCore::ECS::Components::UI::CLayoutGroup::EVerticalAlignment p_alignment
	)
	{
		switch (p_alignment)
		{
		case OvCore::ECS::Components::UI::CLayoutGroup::EVerticalAlignment::TOP:
			return 0.0f;
		case OvCore::ECS::Components::UI::CLayoutGroup::EVerticalAlignment::BOTTOM:
			return 1.0f;
		case OvCore::ECS::Components::UI::CLayoutGroup::EVerticalAlignment::CENTER:
		default:
			return 0.5f;
		}
	}

	std::vector<OvCore::ECS::Components::UI::ClayLayoutChildResult> ProjectSlotsToChildren(
		const ClayLayoutPassResult& p_pass,
		const OvCore::ECS::Components::UI::ClayLayoutSettings& p_settings,
		const std::vector<OvCore::ECS::Components::UI::ClayLayoutChildInput>& p_children,
		const OvMaths::FVector2& p_layoutSize
	)
	{
		using EDirection = OvCore::ECS::Components::UI::CLayoutGroup::EDirection;
		using ClayLayoutChildResult = OvCore::ECS::Components::UI::ClayLayoutChildResult;

		const auto padding = ToClayPadding(p_settings.padding);
		const float spacing = static_cast<float>(ToClaySpacing(p_settings.spacing));
		const float horizontalAlignment = GetHorizontalAlignmentFactor(p_settings.horizontalAlignment);
		const float verticalAlignment = GetVerticalAlignmentFactor(p_settings.verticalAlignment);
		const float availableWidth = std::max(
			p_layoutSize.x - static_cast<float>(padding.left + padding.right),
			0.0f
		);
		const float availableHeight = std::max(
			p_layoutSize.y - static_cast<float>(padding.top + padding.bottom),
			0.0f
		);

		std::vector<OvMaths::FVector2> slotSizes;
		slotSizes.reserve(p_children.size());

		float mainAxisContentSize = 0.0f;
		for (size_t i = 0; i < p_children.size(); ++i)
		{
			const auto slotBox = i < p_pass.slotBoxes.size() ? p_pass.slotBoxes[i] : Clay_BoundingBox{};
			const bool hasResolvedSlot =
				i < p_pass.slotFound.size() &&
				p_pass.slotFound[i] &&
				slotBox.width > 0.0f &&
				slotBox.height > 0.0f;
			const OvMaths::FVector2 slotSize = hasResolvedSlot ?
				OvMaths::FVector2{ slotBox.width, slotBox.height } :
				OvMaths::FVector2{
					ClampNonNegative(p_children[i].preferredSize.x),
					ClampNonNegative(p_children[i].preferredSize.y)
				};

			slotSizes.push_back(slotSize);
			mainAxisContentSize += p_settings.direction == EDirection::HORIZONTAL ? slotSize.x : slotSize.y;
		}

		if (slotSizes.size() > 1)
		{
			mainAxisContentSize += spacing * static_cast<float>(slotSizes.size() - 1);
		}

		const float mainAxisAvailableSize = p_settings.direction == EDirection::HORIZONTAL ?
			availableWidth :
			availableHeight;
		const float mainAxisAlignment = p_settings.direction == EDirection::HORIZONTAL ?
			horizontalAlignment :
			verticalAlignment;
		float mainAxisCursor = (p_settings.direction == EDirection::HORIZONTAL ?
			static_cast<float>(padding.left) :
			static_cast<float>(padding.top)) +
			std::max(mainAxisAvailableSize - mainAxisContentSize, 0.0f) * mainAxisAlignment;

		std::vector<ClayLayoutChildResult> results;
		results.reserve(p_children.size());

		// Clay owns slot sizing. This projection keeps Overload's centered coordinate system deterministic.
		for (size_t i = 0; i < p_children.size(); ++i)
		{
			const auto& slotSize = slotSizes[i];
			OvMaths::FVector2 slotTopLeft;

			if (p_settings.direction == EDirection::HORIZONTAL)
			{
				slotTopLeft = {
					mainAxisCursor,
					static_cast<float>(padding.top) +
						std::max(availableHeight - slotSize.y, 0.0f) * verticalAlignment
				};
				mainAxisCursor += slotSize.x + spacing;
			}
			else
			{
				slotTopLeft = {
					static_cast<float>(padding.left) +
						std::max(availableWidth - slotSize.x, 0.0f) * horizontalAlignment,
					mainAxisCursor
				};
				mainAxisCursor += slotSize.y + spacing;
			}

			const OvMaths::FVector2 childSize = {
				p_settings.controlChildrenWidth ? slotSize.x : ClampNonNegative(p_children[i].preferredSize.x),
				p_settings.controlChildrenHeight ? slotSize.y : ClampNonNegative(p_children[i].preferredSize.y)
			};
			const OvMaths::FVector2 childTopLeft = {
				slotTopLeft.x + std::max(slotSize.x - childSize.x, 0.0f) * horizontalAlignment,
				slotTopLeft.y + std::max(slotSize.y - childSize.y, 0.0f) * verticalAlignment
			};

			results.push_back({
				.actor = p_children[i].actor,
				.offset = ToChildOffset(childTopLeft, childSize, p_layoutSize),
				.size = childSize,
				.valid = p_children[i].actor && childSize.x > 0.0f && childSize.y > 0.0f
			});
		}

		return results;
	}

}

struct OvCore::ECS::Components::UI::ClayLayoutSolverContext::Impl : ClaySolverRuntime
{
};

OvCore::ECS::Components::UI::ClayLayoutSolverContext::ClayLayoutSolverContext() :
m_impl(std::make_unique<Impl>())
{
}

OvCore::ECS::Components::UI::ClayLayoutSolverContext::~ClayLayoutSolverContext() = default;

OvCore::ECS::Components::UI::ClayLayoutSolverContext::ClayLayoutSolverContext(ClayLayoutSolverContext&&) noexcept = default;

OvCore::ECS::Components::UI::ClayLayoutSolverContext& OvCore::ECS::Components::UI::ClayLayoutSolverContext::operator=(ClayLayoutSolverContext&&) noexcept = default;

OvCore::ECS::Components::UI::ClayLayoutMeasurement OvCore::ECS::Components::UI::ClayLayoutSolver::Measure(
	ClayLayoutSolverContext& p_context,
	const ClayLayoutSettings& p_settings,
	const std::vector<ClayLayoutChildInput>& p_children
)
{
	ClayLayoutMeasurement measurement;
	measurement.settings = p_settings;
	measurement.settings.containerSize.x = ClampNonNegative(measurement.settings.containerSize.x);
	measurement.settings.containerSize.y = ClampNonNegative(measurement.settings.containerSize.y);

	auto* context = p_context.m_impl ? p_context.m_impl->GetContext(p_children.size()) : nullptr;
	if (!context)
	{
		return measurement;
	}

	const Clay_SizingAxis measuredWidthSizing = measurement.settings.containerSize.x > 0.0f ?
		MakeFixedSizing(measurement.settings.containerSize.x) :
		MakeFitSizing(kMinimumLayoutSize);
	const Clay_SizingAxis measuredHeightSizing = measurement.settings.containerSize.y > 0.0f ?
		MakeFixedSizing(measurement.settings.containerSize.y) :
		MakeFitSizing(kMinimumLayoutSize);

	const auto preferredPass = RunClayPass(
		context,
		measurement.settings,
		p_children,
		measuredWidthSizing,
		measuredHeightSizing,
		{ kMaximumLayoutSize, kMaximumLayoutSize },
		false
	);

	measurement.preferredSize = preferredPass.size;
	measurement.valid = true;
	return measurement;
}

OvCore::ECS::Components::UI::ClayLayoutSolution OvCore::ECS::Components::UI::ClayLayoutSolver::SolveLayout(
	ClayLayoutSolverContext& p_context,
	const ClayLayoutMeasurement& p_measurement,
	const std::vector<ClayLayoutChildInput>& p_children
)
{
	ClayLayoutSolution solution;

	if (!p_measurement.valid)
	{
		return solution;
	}

	auto* context = p_context.m_impl ? p_context.m_impl->GetContext(p_children.size()) : nullptr;
	if (!context)
	{
		return solution;
	}

	const auto& settings = p_measurement.settings;

	const Clay_SizingAxis finalWidthSizing = settings.containerSize.x > 0.0f ?
		MakeFixedSizing(settings.containerSize.x) :
		MakeFitSizing(p_measurement.preferredSize.x);
	const Clay_SizingAxis finalHeightSizing = settings.containerSize.y > 0.0f ?
		MakeFixedSizing(settings.containerSize.y) :
		MakeFitSizing(p_measurement.preferredSize.y);

	const auto finalPass = RunClayPass(
		context,
		settings,
		p_children,
		finalWidthSizing,
		finalHeightSizing,
		GetRootSize(settings.containerSize, p_measurement.preferredSize),
		true
	);

	solution.result.size = GetRootSize(settings.containerSize, p_measurement.preferredSize);
	solution.result.children = ProjectSlotsToChildren(finalPass, settings, p_children, solution.result.size);

	solution.valid = true;
	return solution;
}

OvCore::ECS::Components::UI::ClayLayoutResult OvCore::ECS::Components::UI::ClayLayoutSolver::Postprocess(
	const ClayLayoutSolution& p_solution
)
{
	if (!p_solution.valid)
	{
		return {};
	}

	return p_solution.result;
}

OvCore::ECS::Components::UI::ClayLayoutResult OvCore::ECS::Components::UI::ClayLayoutSolver::Solve(
	ClayLayoutSolverContext& p_context,
	const ClayLayoutSettings& p_settings,
	const std::vector<ClayLayoutChildInput>& p_children
)
{
	const auto measurement = Measure(p_context, p_settings, p_children);
	const auto solution = SolveLayout(p_context, measurement, p_children);
	return Postprocess(solution);
}
