/*
This file belongs to Ashes.
See LICENSE file in root folder.
*/
#ifndef ___Ashes_DynamicState_HPP___
#define ___Ashes_DynamicState_HPP___
#pragma once

#include "AshesRenderer/AshesRendererPrerequisites.hpp"

namespace ashes
{
	VkPipelineDynamicStateCreateInfo getDynamicState()
	{
		return
		{
			VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO,
			nullptr,
			0u,
			0u,
			nullptr,
		};
	};

	inline bool operator==( VkPipelineDynamicStateCreateInfo const & lhs
		, VkPipelineDynamicStateCreateInfo const & rhs )
	{
		return lhs.flags == rhs.flags
			&& makeArrayView( lhs.pDynamicStates, lhs.dynamicStateCount ) == makeArrayView( rhs.pDynamicStates, rhs.dynamicStateCount );
	}

	inline bool operator!=( VkPipelineDynamicStateCreateInfo const & lhs
		, VkPipelineDynamicStateCreateInfo const & rhs )
	{
		return !( lhs == rhs );
	}

	inline VkPipelineDynamicStateCreateInfo deepCopy( VkPipelineDynamicStateCreateInfo const & rhs
		, VkDynamicStateArray & states )
	{
		VkPipelineDynamicStateCreateInfo result = rhs;
		states = makeVector( rhs.pDynamicStates, rhs.dynamicStateCount );
		result.pDynamicStates = states.data();
		return result;
	}
}

#endif
