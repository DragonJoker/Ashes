/*
This file belongs to Ashes.
See LICENSE file in root folder.
*/
#ifndef ___Ashes_MultisampleState_HPP___
#define ___Ashes_MultisampleState_HPP___
#pragma once

#include "renderer/RendererCommon/AshesRendererPrerequisites.hpp"

namespace ashes
{
	inline VkPipelineMultisampleStateCreateInfo getDefaultMultisampleState()
	{
		return
		{
			VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO,
			nullptr,
			0u,
			VK_SAMPLE_COUNT_1_BIT,
			VK_FALSE,
			1.0f,
			nullptr,
			VK_FALSE,
			VK_FALSE,
		};
	};

	VkPipelineMultisampleStateCreateInfo const & getDeactivatedMultisampleState();

	inline bool operator==( VkPipelineMultisampleStateCreateInfo const & lhs, VkPipelineMultisampleStateCreateInfo const & rhs )
	{
		return lhs.flags == rhs.flags
			&& lhs.rasterizationSamples == rhs.rasterizationSamples
			&& lhs.sampleShadingEnable == rhs.sampleShadingEnable
			&& lhs.minSampleShading == rhs.minSampleShading
			&& lhs.alphaToCoverageEnable == rhs.alphaToCoverageEnable
			&& lhs.alphaToOneEnable == rhs.alphaToOneEnable
			&& ( ( lhs.pSampleMask == nullptr && rhs.pSampleMask == nullptr )
				|| ( lhs.pSampleMask != nullptr && rhs.pSampleMask != nullptr && lhs.pSampleMask == rhs.pSampleMask ) );
	}

	inline bool operator!=( VkPipelineMultisampleStateCreateInfo const & lhs, VkPipelineMultisampleStateCreateInfo const & rhs )
	{
		return !( lhs == rhs );
	}
}

#endif
