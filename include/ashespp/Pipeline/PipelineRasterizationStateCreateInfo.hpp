/*
This file belongs to Ashes.
See LICENSE file in root folder.
*/
#ifndef ___AshesPP_PipelineRasterizationStateCreateInfo_HPP___
#define ___AshesPP_PipelineRasterizationStateCreateInfo_HPP___
#pragma once

#include "ashespp/AshesPPPrerequisites.hpp"

namespace ashes
{
	struct PipelineRasterizationStateCreateInfo
	{
		PipelineRasterizationStateCreateInfo( PipelineRasterizationStateCreateInfo const & ) = delete;
		PipelineRasterizationStateCreateInfo & operator=( PipelineRasterizationStateCreateInfo const & ) = delete;

		PipelineRasterizationStateCreateInfo( VkPipelineRasterizationStateCreateFlags flags = 0u
			, VkBool32 depthClampEnable = VK_FALSE
			, VkBool32 rasterizerDiscardEnable = VK_FALSE
			, VkPolygonMode polygonMode = VK_POLYGON_MODE_FILL
			, VkCullModeFlags cullMode = VK_CULL_MODE_BACK_BIT
			, VkFrontFace frontFace = VK_FRONT_FACE_COUNTER_CLOCKWISE
			, VkBool32 depthBiasEnable = VK_FALSE
			, float depthBiasConstantFactor = 0.0f
			, float depthBiasClamp = 0.0f
			, float depthBiasSlopeFactor = 0.0f
			, float lineWidth = 1.0f )
			: vk
			{
				VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO,
				nullptr,
				flags,
				depthClampEnable,
				rasterizerDiscardEnable,
				polygonMode,
				cullMode,
				frontFace,
				depthBiasEnable,
				depthBiasConstantFactor,
				depthBiasClamp,
				depthBiasSlopeFactor,
				lineWidth,
			}
		{
		}
		
		PipelineRasterizationStateCreateInfo( PipelineRasterizationStateCreateInfo && rhs )noexcept
			: vk
			{
				VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO,
				nullptr,
				rhs.vk.flags,
				rhs.vk.depthClampEnable,
				rhs.vk.rasterizerDiscardEnable,
				rhs.vk.polygonMode,
				rhs.vk.cullMode,
				rhs.vk.frontFace,
				rhs.vk.depthBiasEnable,
				rhs.vk.depthBiasConstantFactor,
				rhs.vk.depthBiasClamp,
				rhs.vk.depthBiasSlopeFactor,
				rhs.vk.lineWidth,
			}
		{
		}
		
		PipelineRasterizationStateCreateInfo & operator=( PipelineRasterizationStateCreateInfo && rhs )noexcept
		{
			vk =
			{
				VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO,
				nullptr,
				rhs.vk.flags,
				rhs.vk.depthClampEnable,
				rhs.vk.rasterizerDiscardEnable,
				rhs.vk.polygonMode,
				rhs.vk.cullMode,
				rhs.vk.frontFace,
				rhs.vk.depthBiasEnable,
				rhs.vk.depthBiasConstantFactor,
				rhs.vk.depthBiasClamp,
				rhs.vk.depthBiasSlopeFactor,
				rhs.vk.lineWidth,
			};

			return *this;
		}

		inline operator VkPipelineRasterizationStateCreateInfo const &()const
		{
			return vk;
		}

		inline VkPipelineRasterizationStateCreateInfo const * operator->()const
		{
			return &vk;
		}

		inline VkPipelineRasterizationStateCreateInfo * operator->()
		{
			return &vk;
		}

	private:
		VkPipelineRasterizationStateCreateInfo vk;
	};
}

#endif
