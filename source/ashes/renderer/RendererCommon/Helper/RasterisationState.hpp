/*
This file belongs to Ashes.
See LICENSE file in root folder.
*/
#ifndef ___Ashes_RasterisationState_HPP___
#define ___Ashes_RasterisationState_HPP___
#pragma once

#include "renderer/RendererCommon/AshesRendererPrerequisites.hpp"

namespace ashes
{
	inline VkPipelineRasterizationStateCreateInfo getDefaultRasterisationState()
	{
		return
		{
			VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO,
			nullptr,
			0u,
			VK_FALSE,
			VK_FALSE,	// rasterizerDiscardEnable.
			VK_POLYGON_MODE_FILL,
			VK_CULL_MODE_BACK_BIT,
			VK_FRONT_FACE_COUNTER_CLOCKWISE,
			VK_FALSE,
			0.0f,
			0.0f,
			0.0f,
			1.0f,
		};
	};

	VkPipelineRasterizationStateCreateInfo const & getDeactivatedRasterizationState();

	inline bool operator==( VkPipelineRasterizationStateCreateInfo const & lhs, VkPipelineRasterizationStateCreateInfo const & rhs )
	{
		return lhs.flags == rhs.flags
			&& lhs.depthClampEnable == rhs.depthClampEnable
			&& lhs.rasterizerDiscardEnable == rhs.rasterizerDiscardEnable
			&& lhs.polygonMode == rhs.polygonMode
			&& lhs.cullMode == rhs.cullMode
			&& lhs.frontFace == rhs.frontFace
			&& lhs.depthBiasEnable == rhs.depthBiasEnable
			&& lhs.depthBiasConstantFactor == rhs.depthBiasConstantFactor
			&& lhs.depthBiasClamp == rhs.depthBiasClamp
			&& lhs.depthBiasSlopeFactor == rhs.depthBiasSlopeFactor
			&& lhs.lineWidth == rhs.lineWidth;
	}

	inline bool operator!=( VkPipelineRasterizationStateCreateInfo const & lhs, VkPipelineRasterizationStateCreateInfo const & rhs )
	{
		return !( lhs == rhs );
	}
}

#endif
