/*
This file belongs to Ashes.
See LICENSE file in root folder.
*/
#ifndef ___Ashes_ViewportState_HPP___
#define ___Ashes_ViewportState_HPP___
#pragma once

#include "AshesRenderer/AshesRendererPrerequisites.hpp"
#include "AshesRenderer/Pipeline/Scissor.hpp"
#include "AshesRenderer/Pipeline/Viewport.hpp"

namespace ashes
{
	/**
	*\~english
	*\brief
	*	The viewport state.
	*\~french
	*\brief
	*	L'état de viewport.
	*/
	VkPipelineViewportStateCreateInfo getViewportState()
	{
		return
		{
			VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO,
			nullptr,
			0u,
			0u,
			nullptr,
			0u,
			nullptr,
		};
	}

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

	inline VkPipelineViewportStateCreateInfo deepCopy( VkPipelineViewportStateCreateInfo const & rhs
		, VkViewportArray & viewports
		, VkScissorArray & scissors )
	{
		VkPipelineViewportStateCreateInfo result = rhs;
		viewports = makeVector( rhs.pViewports, rhs.viewportCount );
		scissors = makeVector( rhs.pScissors, rhs.scissorCount );
		result.pViewports = viewports.data();
		result.pScissors = scissors.data();
		return result;
	}
}

#endif
