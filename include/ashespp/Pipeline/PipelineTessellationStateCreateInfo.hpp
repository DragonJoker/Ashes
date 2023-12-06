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

		PipelineTessellationStateCreateInfo( PipelineTessellationStateCreateInfo const & rhs )
			: vk{ rhs.vk }
		{
		}

		PipelineTessellationStateCreateInfo( PipelineTessellationStateCreateInfo && rhs )noexcept
			: vk{ std::move( rhs.vk ) }
		{
		}

		PipelineTessellationStateCreateInfo & operator=( PipelineTessellationStateCreateInfo const & rhs )
		{
			vk = rhs.vk;
			return *this;
		}

		PipelineTessellationStateCreateInfo & operator=( PipelineTessellationStateCreateInfo && rhs )noexcept
		{
			vk = std::move( rhs.vk );
			return *this;
		}

		operator VkPipelineTessellationStateCreateInfo const &()const
		{
			return vk;
		}

		VkPipelineTessellationStateCreateInfo const * operator->()const
		{
			return &vk;
		}

		VkPipelineTessellationStateCreateInfo * operator->()
		{
			return &vk;
		}

	private:
		VkPipelineTessellationStateCreateInfo vk;
	};
}

#endif
