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
		PipelineMultisampleStateCreateInfo( PipelineMultisampleStateCreateInfo const & ) = delete;
		PipelineMultisampleStateCreateInfo & operator=( PipelineMultisampleStateCreateInfo const & ) = delete;

		PipelineMultisampleStateCreateInfo( VkPipelineMultisampleStateCreateFlags flags = 0u
			, VkSampleCountFlagBits rasterizationSamples = VK_SAMPLE_COUNT_1_BIT
			, VkBool32 sampleShadingEnable = VK_FALSE
			, float minSampleShading = 1.0f
			, Optional< VkSampleMask > sampleMask = Optional< VkSampleMask >{}
			, VkBool32 alphaToCoverageEnable = VK_FALSE
			, VkBool32 alphaToOneEnable = VK_FALSE )
			: sampleMask{ std::move( sampleMask ) }
			, vk
			{
				VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO,
				nullptr,
				flags,
				rasterizationSamples,
				sampleShadingEnable,
				minSampleShading,
				bool( this->sampleMask ) ? &this->sampleMask.value() : nullptr,
				alphaToCoverageEnable,
				alphaToOneEnable,
			}
		{
		}
		
		PipelineMultisampleStateCreateInfo( PipelineMultisampleStateCreateInfo && rhs )
			: sampleMask{ std::move( rhs.sampleMask ) }
			, vk
			{
				VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO,
				nullptr,
				rhs.vk.flags,
				rhs.vk.rasterizationSamples,
				rhs.vk.sampleShadingEnable,
				rhs.vk.minSampleShading,
				bool( this->sampleMask ) ? &this->sampleMask.value() : nullptr,
				rhs.vk.alphaToCoverageEnable,
				rhs.vk.alphaToOneEnable,
			}
		{
		}
		
		PipelineMultisampleStateCreateInfo & operator=( PipelineMultisampleStateCreateInfo && rhs )
		{
			sampleMask = std::move( rhs.sampleMask );
			vk =
			{
				VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO,
				nullptr,
				rhs.vk.flags,
				rhs.vk.rasterizationSamples,
				rhs.vk.sampleShadingEnable,
				rhs.vk.minSampleShading,
				bool( this->sampleMask ) ? &this->sampleMask.value() : nullptr,
				rhs.vk.alphaToCoverageEnable,
				rhs.vk.alphaToOneEnable,
			};

			return *this;
		}

		inline operator VkPipelineMultisampleStateCreateInfo const &()const
		{
			return vk;
		}

		Optional< VkSampleMask > sampleMask;

	private:
		VkPipelineMultisampleStateCreateInfo vk;
	};
}

#endif
