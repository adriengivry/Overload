/**
* @project: Overload
* @author: Overload Tech.
* @licence: MIT
*/

#pragma once

#include <cstdint>

namespace OvRendering::Settings
{
	enum class ERenderPassMask : uint32_t
	{
		NONE				= 0x0,
		PRE_PROCESSING		= 0x1,
		PRE_OPAQUE			= 0x2,
		OPAQUE				= 0x4,
		POST_OPAQUE			= 0x8,
		PRE_TRANSPARENT		= 0x10,
		TRANSPARENT			= 0x20,
		POST_TRANSPARENT	= 0x40,
		POST_PROCESSING		= 0x80,
		UI					= 0x100,
		FIRST				= PRE_PROCESSING,
		LAST				= UI,
		ALL					= ~NONE
	};

	using ERenderPass = ERenderPassMask;

	inline ERenderPassMask operator~ (ERenderPassMask a) { return (ERenderPassMask)~(uint32_t)a; }
	inline ERenderPassMask operator| (ERenderPassMask a, ERenderPassMask b) { return (ERenderPassMask)((uint32_t)a | (uint32_t)b); }
	inline ERenderPassMask operator& (ERenderPassMask a, ERenderPassMask b) { return (ERenderPassMask)((uint32_t)a & (uint32_t)b); }
	inline ERenderPassMask operator^ (ERenderPassMask a, ERenderPassMask b) { return (ERenderPassMask)((uint32_t)a ^ (uint32_t)b); }
	inline ERenderPassMask& operator|= (ERenderPassMask& a, ERenderPassMask b) { return (ERenderPassMask&)((uint32_t&)a |= (uint32_t)b); }
	inline ERenderPassMask& operator&= (ERenderPassMask& a, ERenderPassMask b) { return (ERenderPassMask&)((uint32_t&)a &= (uint32_t)b); }
	inline ERenderPassMask& operator^= (ERenderPassMask& a, ERenderPassMask b) { return (ERenderPassMask&)((uint32_t&)a ^= (uint32_t)b); }
	inline bool IsFlagSet(ERenderPassMask p_flag, ERenderPass p_mask) { return (uint32_t)p_flag & (uint32_t)p_mask; }
}
