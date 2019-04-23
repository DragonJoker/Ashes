/*
This file belongs to Ashes.
See LICENSE file in root folder.
*/
#ifndef ___Ashes_PipelineTessellationStateCreateInfo_HPP___
#define ___Ashes_PipelineTessellationStateCreateInfo_HPP___
#pragma once

#include "ashespp/AshesPPPrerequisites.hpp"

namespace ashes
{
	struct PipelineTessellationStateCreateInfo
	{
		PipelineTessellationStateCreateInfo( PipelineTessellationStateCreateInfo const & ) = delete;
		PipelineTessellationStateCreateInfo & operator=( PipelineTessellationStateCreateInfo const & ) = delete;

		PipelineTessellationStateCreateInfo( VkPipelineTessellationStateCreateFlags flags = 0u
			, uint32_t patchControlPoints = 0u )
			: vk
			{
				VK_STRUCTURE_TYPE_PIPELINE_TESSELLATION_STATE_CREATE_INFO,
				nullptr,
				flags,
				patchControlPoints,
			}
		{
		}
		
		PipelineTessellationStateCreateInfo( PipelineTessellationStateCreateInfo && rhs )
			: vk
			{
				VK_STRUCTURE_TYPE_PIPELINE_TESSELLATION_STATE_CREATE_INFO,
				nullptr,
				rhs.vk.flags,
				rhs.vk.patchControlPoints,
			}
		{
		}
		
		PipelineTessellationStateCreateInfo & operator=( PipelineTessellationStateCreateInfo && rhs )
		{
			vk =
			{
				VK_STRUCTURE_TYPE_PIPELINE_TESSELLATION_STATE_CREATE_INFO,
				nullptr,
				rhs.vk.flags,
				rhs.vk.patchControlPoints,
			};

			return *this;
		}

		inline operator VkPipelineTessellationStateCreateInfo const &()const
		{
			return vk;
		}

	private:
		VkPipelineTessellationStateCreateInfo vk;
	};
}

#endif
