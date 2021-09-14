/*
This file belongs to Ashes.
See LICENSE file in root folder.
*/
#ifndef ___Ashes_InputAssemblyState_HPP___
#define ___Ashes_InputAssemblyState_HPP___
#pragma once

#include "renderer/RendererCommon/AshesRendererPrerequisites.hpp"

namespace ashes
{
	inline VkPipelineInputAssemblyStateCreateInfo getDefaultInputAssemblyState()
	{
		return
		{
			VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO,
			nullptr,
			0u,
			VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST,
			VK_FALSE,
		};
	}

	inline bool operator==( VkPipelineInputAssemblyStateCreateInfo const & lhs, VkPipelineInputAssemblyStateCreateInfo const & rhs )
	{
		return lhs.flags == rhs.flags
			&& lhs.topology == rhs.topology
			&& lhs.primitiveRestartEnable == rhs.primitiveRestartEnable;
	}

	inline bool operator!=( VkPipelineInputAssemblyStateCreateInfo const & lhs, VkPipelineInputAssemblyStateCreateInfo const & rhs )
	{
		return !( lhs == rhs );
	}
}

#endif
