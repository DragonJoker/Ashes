/*
This file belongs to Ashes.
See LICENSE file in root folder.
*/
#ifndef ___Ashes_ViewportState_HPP___
#define ___Ashes_ViewportState_HPP___
#pragma once

#include "renderer/RendererCommon/AshesRendererPrerequisites.hpp"

#include <ashes/common/ArrayView.hpp>

namespace ashes
{
	/**
	*\brief
	*	The viewport state.
	*/
	inline VkPipelineViewportStateCreateInfo getDefaultViewportState()
	{
		return
		{
			VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO,
			nullptr,
			0u,
			1u,
			nullptr,
			1u,
			nullptr,
		};
	}

	VkPipelineViewportStateCreateInfo const & getDeactivatedViewportState();

	inline bool operator==( VkPipelineViewportStateCreateInfo const & lhs
		, VkPipelineViewportStateCreateInfo const & rhs )
	{
		return lhs.flags == rhs.flags
			&& makeArrayView( lhs.pViewports, lhs.viewportCount ) == makeArrayView( rhs.pViewports, rhs.viewportCount )
			&& makeArrayView( lhs.pScissors, lhs.scissorCount ) == makeArrayView( rhs.pScissors, rhs.scissorCount );
	}

	inline bool operator!=( VkPipelineViewportStateCreateInfo const & lhs
		, VkPipelineViewportStateCreateInfo const & rhs )
	{
		return !( lhs == rhs );
	}
}

#endif
