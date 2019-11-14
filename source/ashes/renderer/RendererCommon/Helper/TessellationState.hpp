/*
This file belongs to Ashes.
See LICENSE file in root folder.
*/
#ifndef ___Ashes_TessellationState_HPP___
#define ___Ashes_TessellationState_HPP___
#pragma once

#include "ashes/renderer/RendererCommon/AshesRendererPrerequisites.hpp"

namespace ashes
{
	inline VkPipelineTessellationStateCreateInfo getDefaultTessellationState()
	{
		return
		{
			VK_STRUCTURE_TYPE_PIPELINE_TESSELLATION_STATE_CREATE_INFO,
			nullptr,
			0u,
			0u,
		};
	};

	VkPipelineTessellationStateCreateInfo const & getDeactivatedTessellationState();

	inline bool operator==( VkPipelineTessellationStateCreateInfo const & lhs, VkPipelineTessellationStateCreateInfo const & rhs )
	{
		return lhs.flags == rhs.flags
			&& lhs.patchControlPoints == rhs.patchControlPoints;
	}

	inline bool operator!=( VkPipelineTessellationStateCreateInfo const & lhs, VkPipelineTessellationStateCreateInfo const & rhs )
	{
		return !( lhs == rhs );
	}
}

#endif
