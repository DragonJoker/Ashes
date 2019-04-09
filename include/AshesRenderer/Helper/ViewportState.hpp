/*
This file belongs to Ashes.
See LICENSE file in root folder.
*/
#ifndef ___Ashes_ViewportState_HPP___
#define ___Ashes_ViewportState_HPP___
#pragma once

#include "AshesRenderer/AshesRendererPrerequisites.hpp"

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

	inline bool operator==( VkViewport const & lhs
		, VkViewport const & rhs )
	{
		return lhs.width == rhs.width
			&& lhs.height == rhs.height
			&& lhs.x == rhs.x
			&& lhs.y == rhs.y
			&& lhs.minDepth == rhs.minDepth
			&& lhs.maxDepth == rhs.maxDepth;
	}

	inline bool operator!=( VkViewport const & lhs
		, VkViewport const & rhs )
	{
		return !( lhs == rhs );
	}

	inline bool operator==( VkOffset2D const & lhs
		, VkOffset2D const & rhs )
	{
		return lhs.x == rhs.x
			&& lhs.y == rhs.y;
	}

	inline bool operator!=( VkOffset2D const & lhs
		, VkOffset2D const & rhs )
	{
		return !( lhs == rhs );
	}

	inline bool operator==( VkExtent2D const & lhs
		, VkExtent2D const & rhs )
	{
		return lhs.width == rhs.width
			&& lhs.height == rhs.height;
	}

	inline bool operator!=( VkExtent2D const & lhs
		, VkExtent2D const & rhs )
	{
		return !( lhs == rhs );
	}

	inline bool operator==( VkRect2D const & lhs
		, VkRect2D const & rhs )
	{
		return lhs.offset == rhs.offset
			&& lhs.extent == rhs.extent;
	}

	inline bool operator!=( VkRect2D const & lhs
		, VkRect2D const & rhs )
	{
		return !( lhs == rhs );
	}

	inline VkViewport deepCopy( VkViewport const & rhs )
	{
		return rhs;
	}

	inline VkRect2D deepCopy( VkRect2D const & rhs )
	{
		return rhs;
	}

	inline VkPipelineViewportStateCreateInfo deepCopy( VkPipelineViewportStateCreateInfo const & rhs
		, VkViewportArray & viewports
		, VkScissorArray & scissors )
	{
		VkPipelineViewportStateCreateInfo result = rhs;

		if ( rhs.pViewports )
		{
			viewports = makeVector( rhs.pViewports, rhs.viewportCount );
		}

		if ( rhs.pScissors )
		{
			scissors = makeVector( rhs.pScissors, rhs.scissorCount );
		}

		result.pViewports = viewports.data();
		result.pScissors = scissors.data();
		return result;
	}
}

#endif
