/*
This file belongs to Ashes.
See LICENSE file in root folder.
*/
#ifndef ___AshesPP_PipelineViewportStateCreateInfo_HPP___
#define ___AshesPP_PipelineViewportStateCreateInfo_HPP___
#pragma once

#include "ashespp/AshesPPPrerequisites.hpp"

namespace ashes
{
	struct PipelineViewportStateCreateInfo
	{
		PipelineViewportStateCreateInfo( PipelineViewportStateCreateInfo const & ) = delete;
		PipelineViewportStateCreateInfo & operator=( PipelineViewportStateCreateInfo const & ) = delete;

		PipelineViewportStateCreateInfo( VkPipelineViewportStateCreateFlags flags
			, uint32_t viewportCount
			, VkViewportArray viewports
			, uint32_t scissorCount
			, VkScissorArray scissors )
			: viewports{ std::move( viewports ) }
			, scissors{ std::move( scissors ) }
			, vk
			{
				VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO,
				nullptr,
				flags,
				viewportCount,
				this->viewports.data(),
				scissorCount,
				this->scissors.data(),
			}
		{
		}

		PipelineViewportStateCreateInfo( VkPipelineViewportStateCreateFlags flags = 0u
			, VkViewportArray viewports = {}
			, VkScissorArray scissors = {} )
			: PipelineViewportStateCreateInfo
			{
				flags,
				std::max( 1u, uint32_t( viewports.size() ) ),
				viewports,
				std::max( 1u, uint32_t( scissors.size() ) ),
				scissors,
			}
		{
		}

		PipelineViewportStateCreateInfo( PipelineViewportStateCreateInfo && rhs )noexcept
			: viewports{ std::move( rhs.viewports ) }
			, scissors{ std::move( rhs.scissors ) }
			, vk
			{
				VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO,
				nullptr,
				rhs.vk.flags,
				rhs.vk.viewportCount,
				this->viewports.data(),
				rhs.vk.scissorCount,
				this->scissors.data(),
			}
		{
		}

		PipelineViewportStateCreateInfo & operator=( PipelineViewportStateCreateInfo && rhs )noexcept
		{
			viewports = std::move( rhs.viewports );
			scissors = std::move( rhs.scissors );
			vk =
			{
				VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO,
				nullptr,
				rhs.vk.flags,
				rhs.vk.viewportCount,
				this->viewports.data(),
				rhs.vk.scissorCount,
				this->scissors.data(),
			};

			return *this;
		}

		inline operator VkPipelineViewportStateCreateInfo const &()const
		{
			return vk;
		}

		inline VkPipelineViewportStateCreateInfo const * operator->()const
		{
			return &vk;
		}

		inline VkPipelineViewportStateCreateInfo * operator->()
		{
			return &vk;
		}

	private:
		VkViewportArray viewports;
		VkScissorArray scissors;
		VkPipelineViewportStateCreateInfo vk;
	};
}

#endif
