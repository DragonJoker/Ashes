/*
This file belongs to Ashes.
See LICENSE file in root folder.
*/
#ifndef ___Ashes_StencilOpState_HPP___
#define ___Ashes_StencilOpState_HPP___
#pragma once

#include "renderer/RendererCommon/AshesRendererPrerequisites.hpp"

namespace ashes
{
	inline VkStencilOpState getStencilOpState()
	{
		return { VK_STENCIL_OP_KEEP
			, VK_STENCIL_OP_KEEP
			, VK_STENCIL_OP_KEEP
			, VK_COMPARE_OP_ALWAYS
			, 0xFFFFFFFFu
			, 0xFFFFFFFFu
			, 0u };
	}

	inline bool operator==( VkStencilOpState const & lhs, VkStencilOpState const & rhs )
	{
		return lhs.failOp == rhs.failOp
			&& lhs.passOp == rhs.passOp
			&& lhs.depthFailOp == rhs.depthFailOp
			&& lhs.compareOp == rhs.compareOp
			&& lhs.compareMask == rhs.compareMask
			&& lhs.writeMask == rhs.writeMask
			&& lhs.reference == rhs.reference;
	}

	inline bool operator!=( VkStencilOpState const & lhs, VkStencilOpState const & rhs )
	{
		return !( lhs == rhs );
	}
}

#endif
