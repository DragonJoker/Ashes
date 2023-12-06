/*
This file belongs to Ashes.
See LICENSE file in root folder.
*/
#ifndef ___AshesPP_ComputePipelineCreateInfo_HPP___
#define ___AshesPP_ComputePipelineCreateInfo_HPP___
#pragma once

#include "PipelineShaderStageCreateInfo.hpp"

namespace ashes
{
	/**
	*\brief
	*	Holds the data used to create a compute pipeline.
	*/
	struct ComputePipelineCreateInfo
	{
		ComputePipelineCreateInfo( VkPipelineCreateFlags flags
			, PipelineShaderStageCreateInfo pstage
			, VkPipelineLayout layout
			, VkPipeline basePipelineHandle = {}
			, int32_t basePipelineIndex = {} )
			: stage{ std::move( pstage ) }
			, vk{ VK_STRUCTURE_TYPE_COMPUTE_PIPELINE_CREATE_INFO
				, nullptr
				, flags
				, stage
				, layout
				, basePipelineHandle
				, basePipelineIndex
			}
		{
		}

		ComputePipelineCreateInfo( ComputePipelineCreateInfo const & rhs )
			: stage{ rhs.stage }
			, vk{ VK_STRUCTURE_TYPE_COMPUTE_PIPELINE_CREATE_INFO
				, rhs.vk.pNext
				, rhs.vk.flags
				, stage
				, rhs.vk.layout
				, rhs.vk.basePipelineHandle
				, rhs.vk.basePipelineIndex }
		{
		}

		ComputePipelineCreateInfo( ComputePipelineCreateInfo && rhs )noexcept
			: stage{ rhs.stage }
			, vk{ VK_STRUCTURE_TYPE_COMPUTE_PIPELINE_CREATE_INFO
			, rhs.vk.pNext
			, rhs.vk.flags
			, stage
			, rhs.vk.layout
			, rhs.vk.basePipelineHandle
			, rhs.vk.basePipelineIndex }
		{
		}

		ComputePipelineCreateInfo & operator=( ComputePipelineCreateInfo const & rhs )
		{
			stage = std::move( rhs.stage );
			vk = { VK_STRUCTURE_TYPE_COMPUTE_PIPELINE_CREATE_INFO
				, rhs.vk.pNext
				, rhs.vk.flags
				, stage
				, rhs.vk.layout
				, rhs.vk.basePipelineHandle
				, rhs.vk.basePipelineIndex };

			return *this;
		}

		ComputePipelineCreateInfo & operator=( ComputePipelineCreateInfo && rhs )noexcept
		{
			stage = std::move( rhs.stage );
			vk = { VK_STRUCTURE_TYPE_COMPUTE_PIPELINE_CREATE_INFO
				, rhs.vk.pNext
				, rhs.vk.flags
				, stage
				, rhs.vk.layout
				, rhs.vk.basePipelineHandle
				, rhs.vk.basePipelineIndex };

			return *this;
		}

		operator VkComputePipelineCreateInfo const & ()const
		{
			return vk;
		}

		VkComputePipelineCreateInfo const * operator->()const
		{
			return &vk;
		}

		VkComputePipelineCreateInfo * operator->()
		{
			return &vk;
		}

		PipelineShaderStageCreateInfo stage;

	private:
		VkComputePipelineCreateInfo vk;
	};
}

#endif
