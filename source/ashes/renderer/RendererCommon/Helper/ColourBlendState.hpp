/*
This file belongs to Ashes.
See LICENSE file in root folder.
*/
#ifndef ___Ashes_ColourBlendState_HPP___
#define ___Ashes_ColourBlendState_HPP___
#pragma once

#include "ColourBlendStateAttachment.hpp"

namespace ashes
{
	inline VkPipelineColorBlendStateCreateInfo getDefaultColorBlendState( VkPipelineColorBlendAttachmentStateArray const & attachments
		, void * pNext = nullptr )
	{
		return { VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO
			, pNext
			, 0u
			, VK_FALSE
			, VK_LOGIC_OP_COPY
			, uint32_t( attachments.size() )
			, attachments.data()
			, { 0.0f, 0.0f, 0.0f, 0.0f } };
	}

	inline bool operator==( VkPipelineColorBlendStateCreateInfo const & lhs
		, VkPipelineColorBlendStateCreateInfo const & rhs )
	{
		auto result = lhs.flags == rhs.flags
			&& lhs.logicOpEnable == rhs.logicOpEnable
			&& lhs.logicOp == rhs.logicOp
			&& lhs.blendConstants[0] == rhs.blendConstants[0]
			&& lhs.blendConstants[1] == rhs.blendConstants[1]
			&& lhs.blendConstants[2] == rhs.blendConstants[2]
			&& lhs.blendConstants[3] == rhs.blendConstants[3]
			&& lhs.attachmentCount == rhs.attachmentCount;

		for ( size_t i = 0; i < lhs.attachmentCount && result; ++i )
		{
			result = lhs.pAttachments[i] == rhs.pAttachments[i];
		}

		return result;
	}

	inline bool operator!=( VkPipelineColorBlendStateCreateInfo const & lhs
		, VkPipelineColorBlendStateCreateInfo const & rhs )
	{
		return !( lhs == rhs );
	}
}

#endif
