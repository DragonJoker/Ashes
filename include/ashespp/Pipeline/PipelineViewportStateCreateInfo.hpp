/*
This file belongs to Ashes.
See LICENSE file in root folder.
*/
#ifndef ___Ashes_PipelineViewportStateCreateInfo_HPP___
#define ___Ashes_PipelineViewportStateCreateInfo_HPP___
#pragma once

#include "ashespp/AshesPPPrerequisites.hpp"

namespace ashes
{
	struct PipelineViewportStateCreateInfo
	{
		PipelineViewportStateCreateInfo( PipelineViewportStateCreateInfo const & ) = delete;
		PipelineViewportStateCreateInfo & operator=( PipelineViewportStateCreateInfo const & ) = delete;

		PipelineViewportStateCreateInfo( VkPipelineViewportStateCreateFlags flags = 0u
			, uint32_t viewportCount = 1u
			, VkViewportArray viewports = {}
			, uint32_t scissorCount = 1u
			, VkScissorArray scissors = {} )
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

		PipelineViewportStateCreateInfo( PipelineViewportStateCreateInfo && rhs )
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

		PipelineViewportStateCreateInfo & operator=( PipelineViewportStateCreateInfo && rhs )
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

	private:
		VkViewportArray viewports;
		VkScissorArray scissors;
		VkPipelineViewportStateCreateInfo vk;
	};
}

#endif
