/*
This file belongs to Ashes.
See LICENSE file in root folder.
*/
#ifndef ___AshesPP_PipelineColorBlendStateCreateInfo_HPP___
#define ___AshesPP_PipelineColorBlendStateCreateInfo_HPP___
#pragma once

#include "ashespp/AshesPPPrerequisites.hpp"

namespace ashes
{
	struct PipelineColorBlendStateCreateInfo
	{
		PipelineColorBlendStateCreateInfo( VkPipelineColorBlendStateCreateFlags flags = 0u
			, VkBool32 logicOpEnable = VK_FALSE
			, VkLogicOp logicOp = VK_LOGIC_OP_COPY
			, VkPipelineColorBlendAttachmentStateArray pattachments = { { VK_FALSE
				, VK_BLEND_FACTOR_ONE
				, VK_BLEND_FACTOR_ZERO
				, VK_BLEND_OP_ADD
				, VK_BLEND_FACTOR_ONE
				, VK_BLEND_FACTOR_ZERO
				, VK_BLEND_OP_ADD
				, VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT } }
			, std::array< float, 4u > blendConstants = { 0.0f, 0.0f, 0.0f, 0.0f } )
			: attachments{ std::move( pattachments ) }
			, vk{ VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO
				, nullptr
				, flags
				, logicOpEnable
				, logicOp
				, uint32_t( attachments.size() )
				, attachments.data()
				, { blendConstants[0]
					, blendConstants[1]
					, blendConstants[2]
					, blendConstants[3] } }
		{
		}

		PipelineColorBlendStateCreateInfo( PipelineColorBlendStateCreateInfo const & rhs )
			: attachments{ rhs.attachments }
			, vk{ VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO
				, rhs->pNext
				, rhs->flags
				, rhs->logicOpEnable
				, rhs->logicOp
				, uint32_t( attachments.size() )
				, attachments.data()
				, { rhs.vk.blendConstants[0]
					, rhs.vk.blendConstants[1]
					, rhs.vk.blendConstants[2]
					, rhs.vk.blendConstants[3] } }
		{
		}

		PipelineColorBlendStateCreateInfo( PipelineColorBlendStateCreateInfo && rhs )noexcept
			: attachments{ std::move( rhs.attachments ) }
			, vk{ VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO
				, rhs->pNext
				, rhs->flags
				, rhs->logicOpEnable
				, rhs->logicOp
				, uint32_t( attachments.size() )
				, attachments.data()
				, { rhs.vk.blendConstants[0]
					, rhs.vk.blendConstants[1]
					, rhs.vk.blendConstants[2]
					, rhs.vk.blendConstants[3] } }
		{
		}

		PipelineColorBlendStateCreateInfo & operator=( PipelineColorBlendStateCreateInfo const & rhs )
		{
			attachments = rhs.attachments;
			vk = { VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO
				, rhs.vk.pNext
				, rhs.vk.flags
				, rhs.vk.logicOpEnable
				, rhs.vk.logicOp
				, uint32_t( attachments.size() )
				, attachments.data()
				, { rhs.vk.blendConstants[0]
					, rhs.vk.blendConstants[1]
					, rhs.vk.blendConstants[2]
					, rhs.vk.blendConstants[3] } };

			return *this;
		}

		PipelineColorBlendStateCreateInfo & operator=( PipelineColorBlendStateCreateInfo && rhs )noexcept
		{
			attachments = std::move( rhs.attachments );
			vk = { VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO
				, rhs.vk.pNext
				, rhs.vk.flags
				, rhs.vk.logicOpEnable
				, rhs.vk.logicOp
				, uint32_t( attachments.size() )
				, attachments.data()
				, { rhs.vk.blendConstants[0]
					, rhs.vk.blendConstants[1]
					, rhs.vk.blendConstants[2]
					, rhs.vk.blendConstants[3] } };

			return *this;
		}

		operator VkPipelineColorBlendStateCreateInfo const &()const
		{
			return vk;
		}

		VkPipelineColorBlendStateCreateInfo const * operator->()const
		{
			return &vk;
		}

		VkPipelineColorBlendStateCreateInfo * operator->()
		{
			return &vk;
		}

		VkPipelineColorBlendAttachmentStateArray attachments;

	private:
		VkPipelineColorBlendStateCreateInfo vk;
	};
}

#endif
