/*
This file belongs to Ashes.
See LICENSE file in root folder.
*/
#ifndef ___AshesPP_PipelineDynamicStateCreateInfo_HPP___
#define ___AshesPP_PipelineDynamicStateCreateInfo_HPP___
#pragma once

#include "ashespp/AshesPPPrerequisites.hpp"

namespace ashes
{
	struct PipelineDynamicStateCreateInfo
	{
		PipelineDynamicStateCreateInfo( PipelineDynamicStateCreateInfo const & ) = delete;
		PipelineDynamicStateCreateInfo & operator=( PipelineDynamicStateCreateInfo const & ) = delete;

		PipelineDynamicStateCreateInfo( VkPipelineDynamicStateCreateFlags flags = 0u
			, VkDynamicStateArray pdynamicStates = {} )
			: dynamicStates{ std::move( pdynamicStates ) }
			, vk{ VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO
				, nullptr
				, flags
				, uint32_t( dynamicStates.size() )
				, dynamicStates.data() }
		{
		}

		PipelineDynamicStateCreateInfo( PipelineDynamicStateCreateInfo && rhs )noexcept
			: dynamicStates{ std::move( rhs.dynamicStates ) }
			, vk{ VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO
				, rhs.vk.pNext
				, rhs.vk.flags
				, uint32_t( dynamicStates.size() )
				, dynamicStates.data() }
		{
		}

		PipelineDynamicStateCreateInfo & operator=( PipelineDynamicStateCreateInfo && rhs )noexcept
		{
			dynamicStates = std::move( rhs.dynamicStates );
			vk = { VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO
				, rhs.vk.pNext
				, rhs.vk.flags
				, uint32_t( dynamicStates.size() )
				, dynamicStates.data() };
			return *this;
		}

		operator VkPipelineDynamicStateCreateInfo const &()const
		{
			return vk;
		}

		VkPipelineDynamicStateCreateInfo const * operator->()const
		{
			return &vk;
		}

		VkPipelineDynamicStateCreateInfo * operator->()
		{
			return &vk;
		}

	private:
		VkDynamicStateArray dynamicStates;
		VkPipelineDynamicStateCreateInfo vk;
	};
}

#endif
