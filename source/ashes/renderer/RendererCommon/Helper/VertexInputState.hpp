/*
This file belongs to Ashes.
See LICENSE file in root folder.
*/
#ifndef ___Ashes_VertexInputState_HPP___
#define ___Ashes_VertexInputState_HPP___
#pragma once

#include "renderer/RendererCommon/AshesRendererPrerequisites.hpp"

namespace ashes
{
	inline VkPipelineVertexInputStateCreateInfo getVertexInputState()
	{
		return
		{
			VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO,
			nullptr,
			0u,
			0u,
			nullptr,
			0u,
			nullptr,
		};
	};

	inline bool operator==( VkPipelineVertexInputStateCreateInfo const & lhs
		, VkPipelineVertexInputStateCreateInfo const & rhs )
	{
		return lhs.flags == rhs.flags
			&& makeArrayView( lhs.pVertexBindingDescriptions, lhs.vertexBindingDescriptionCount ) == makeArrayView( rhs.pVertexBindingDescriptions, rhs.vertexBindingDescriptionCount )
			&& makeArrayView( lhs.pVertexAttributeDescriptions, lhs.vertexAttributeDescriptionCount ) == makeArrayView( rhs.pVertexAttributeDescriptions, rhs.vertexAttributeDescriptionCount );
	}

	inline bool operator!=( VkPipelineVertexInputStateCreateInfo const & lhs
		, VkPipelineVertexInputStateCreateInfo const & rhs )
	{
		return !( lhs == rhs );
	}

	inline bool isEmpty( VkPipelineVertexInputStateCreateInfo const & value )
	{
		return value.vertexAttributeDescriptionCount == 0u
			&& value.vertexBindingDescriptionCount == 0u;
	}
}

#endif
