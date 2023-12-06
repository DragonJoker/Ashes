/*
This file belongs to Ashes.
See LICENSE file in root folder.
*/
#ifndef ___AshesPP_PipelineMultisampleStateCreateInfo_HPP___
#define ___AshesPP_PipelineMultisampleStateCreateInfo_HPP___
#pragma once

#include "ashespp/AshesPPPrerequisites.hpp"

namespace ashes
{
	struct PipelineMultisampleStateCreateInfo
	{
		PipelineMultisampleStateCreateInfo( VkPipelineMultisampleStateCreateFlags flags = 0u
			, VkSampleCountFlagBits rasterizationSamples = VK_SAMPLE_COUNT_1_BIT
			, VkBool32 sampleShadingEnable = VK_FALSE
			, float minSampleShading = 1.0f
			, Optional< VkSampleMask > psampleMask = Optional< VkSampleMask >{}
			, VkBool32 alphaToCoverageEnable = VK_FALSE
			, VkBool32 alphaToOneEnable = VK_FALSE )
			: sampleMask{ std::move( psampleMask ) }
			, vk{ VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO
				, nullptr
				, flags
				, rasterizationSamples
				, sampleShadingEnable
				, minSampleShading
				, ( bool( sampleMask ) ? &sampleMask.value() : nullptr )
				, alphaToCoverageEnable
				, alphaToOneEnable }
		{
		}

		PipelineMultisampleStateCreateInfo( PipelineMultisampleStateCreateInfo const & rhs )
			: sampleMask{ rhs.sampleMask }
			, vk{ VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO
				, rhs.vk.pNext
				, rhs.vk.flags
				, rhs.vk.rasterizationSamples
				, rhs.vk.sampleShadingEnable
				, rhs.vk.minSampleShading
				, ( bool( sampleMask ) ? &sampleMask.value() : nullptr )
				, rhs.vk.alphaToCoverageEnable
				, rhs.vk.alphaToOneEnable }
		{
		}

		PipelineMultisampleStateCreateInfo( PipelineMultisampleStateCreateInfo && rhs )noexcept
			: sampleMask{ std::move( rhs.sampleMask ) }
			, vk{ VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO
				, rhs.vk.pNext
				, rhs.vk.flags
				, rhs.vk.rasterizationSamples
				, rhs.vk.sampleShadingEnable
				, rhs.vk.minSampleShading
				, ( bool( sampleMask ) ? &sampleMask.value() : nullptr )
				, rhs.vk.alphaToCoverageEnable
				, rhs.vk.alphaToOneEnable }
		{
		}

		PipelineMultisampleStateCreateInfo & operator=( PipelineMultisampleStateCreateInfo const & rhs )
		{
			sampleMask = rhs.sampleMask;
			vk = { VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO
				, rhs.vk.pNext
				, rhs.vk.flags
				, rhs.vk.rasterizationSamples
				, rhs.vk.sampleShadingEnable
				, rhs.vk.minSampleShading
				, ( bool( sampleMask ) ? &sampleMask.value() : nullptr )
				, rhs.vk.alphaToCoverageEnable
				, rhs.vk.alphaToOneEnable };
			return *this;
		}

		PipelineMultisampleStateCreateInfo & operator=( PipelineMultisampleStateCreateInfo && rhs )noexcept
		{
			sampleMask = std::move( rhs.sampleMask );
			vk = { VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO
				, rhs.vk.pNext
				, rhs.vk.flags
				, rhs.vk.rasterizationSamples
				, rhs.vk.sampleShadingEnable
				, rhs.vk.minSampleShading
				, ( bool( sampleMask ) ? &sampleMask.value() : nullptr )
				, rhs.vk.alphaToCoverageEnable
				, rhs.vk.alphaToOneEnable };
			return *this;
		}

		operator VkPipelineMultisampleStateCreateInfo const &()const
		{
			return vk;
		}

		VkPipelineMultisampleStateCreateInfo const * operator->()const
		{
			return &vk;
		}

		VkPipelineMultisampleStateCreateInfo * operator->()
		{
			return &vk;
		}

		Optional< VkSampleMask > sampleMask;

	private:
		VkPipelineMultisampleStateCreateInfo vk;
	};
}

#endif
