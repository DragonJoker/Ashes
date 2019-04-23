/*
This file belongs to Ashes.
See LICENSE file in root folder.
*/
#ifndef ___Ashes_DynamicState_HPP___
#define ___Ashes_DynamicState_HPP___
#pragma once

#include "renderer/RendererCommon/AshesRendererPrerequisites.hpp"

#include <algorithm>

namespace ashes
{
	inline VkPipelineDynamicStateCreateInfo getDefaultDynamicState()
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

	VkPipelineDynamicStateCreateInfo const & getDeactivatedDynamicState();

	template< typename ItType >
	bool hasDynamicState( ItType begin
		, ItType end
		, VkDynamicState state )
	{
		return end != std::find( begin, end, state );
	}

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

	inline VkDynamicState deepCopy( VkDynamicState const & rhs )
	{
		return rhs;
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
