/*
This file belongs to Ashes.
See LICENSE file in root folder.
*/
#ifndef ___AshesPP_PipelineTessellationStateCreateInfo_HPP___
#define ___AshesPP_PipelineTessellationStateCreateInfo_HPP___
#pragma once

#include "ashespp/AshesPPPrerequisites.hpp"

namespace ashes
{
	struct PipelineTessellationStateCreateInfo
	{
		PipelineTessellationStateCreateInfo( VkPipelineTessellationStateCreateFlags flags = 0u
			, uint32_t patchControlPoints = 0u )
			: vk{ VK_STRUCTURE_TYPE_PIPELINE_TESSELLATION_STATE_CREATE_INFO
				, nullptr
				, flags
				, patchControlPoints }
		{
		}

		operator VkPipelineTessellationStateCreateInfo const &()const noexcept
		{
			return vk;
		}

		VkPipelineTessellationStateCreateInfo const * operator->()const noexcept
		{
			return &vk;
		}

		VkPipelineTessellationStateCreateInfo * operator->()noexcept
		{
			return &vk;
		}

	private:
		VkPipelineTessellationStateCreateInfo vk;
	};
}

#endif
