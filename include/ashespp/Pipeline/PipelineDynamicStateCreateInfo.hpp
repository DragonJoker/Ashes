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
			, VkDynamicStateArray dynamicStates = {} )
			: dynamicStates{ std::move( dynamicStates ) }
			, vk
			{
				VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO,
				nullptr,
				flags,
				uint32_t( this->dynamicStates.size() ),
				this->dynamicStates.data(),
			}
		{
		}

		PipelineDynamicStateCreateInfo( PipelineDynamicStateCreateInfo && rhs )
			: dynamicStates{ std::move( rhs.dynamicStates ) }
			, vk
			{
				VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO,
				nullptr,
				rhs.vk.flags,
				uint32_t( this->dynamicStates.size() ),
				this->dynamicStates.data(),
			}
		{
		}

		PipelineDynamicStateCreateInfo & operator=( PipelineDynamicStateCreateInfo && rhs )
		{
			dynamicStates = std::move( rhs.dynamicStates );
			vk =
			{
				VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO,
				nullptr,
				rhs.vk.flags,
				uint32_t( this->dynamicStates.size() ),
				this->dynamicStates.data(),
			};

			return *this;
		}

		inline operator VkPipelineDynamicStateCreateInfo const &()const
		{
			return vk;
		}

	private:
		VkDynamicStateArray dynamicStates;
		VkPipelineDynamicStateCreateInfo vk;
	};
}

#endif
