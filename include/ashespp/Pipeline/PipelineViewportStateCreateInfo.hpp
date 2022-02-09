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
			, VkViewportArray pviewports
			, uint32_t scissorCount
			, VkScissorArray pscissors )
			: viewports{ std::move( pviewports ) }
			, scissors{ std::move( pscissors ) }
			, vk
			{
				VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO,
				nullptr,
				flags,
				viewportCount,
				viewports.data(),
				scissorCount,
				scissors.data(),
			}
		{
		}

		PipelineViewportStateCreateInfo( VkPipelineViewportStateCreateFlags flags = 0u
			, VkViewportArray pviewports = {}
			, VkScissorArray pscissors = {} )
			: PipelineViewportStateCreateInfo{ flags
				, std::max( 1u, uint32_t( pviewports.size() ) )
				, pviewports
				, std::max( 1u, uint32_t( pscissors.size() ) )
				, pscissors }
		{
		}

		PipelineViewportStateCreateInfo( PipelineViewportStateCreateInfo && rhs )noexcept
			: viewports{ std::move( rhs.viewports ) }
			, scissors{ std::move( rhs.scissors ) }
			, vk{ VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO
				, rhs.vk.pNext
				, rhs.vk.flags
				, rhs.vk.viewportCount
				, viewports.data()
				, rhs.vk.scissorCount
				, scissors.data() }
		{
		}

		PipelineViewportStateCreateInfo & operator=( PipelineViewportStateCreateInfo && rhs )noexcept
		{
			viewports = std::move( rhs.viewports );
			scissors = std::move( rhs.scissors );
			vk = { VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO
				, rhs.vk.pNext
				, rhs.vk.flags
				, rhs.vk.viewportCount
				, viewports.data()
				, rhs.vk.scissorCount
				, scissors.data() };

			return *this;
		}

		operator VkPipelineViewportStateCreateInfo const &()const
		{
			return vk;
		}

		VkPipelineViewportStateCreateInfo const * operator->()const
		{
			return &vk;
		}

		VkPipelineViewportStateCreateInfo * operator->()
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
