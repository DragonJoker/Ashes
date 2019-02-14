/*
This file belongs to Ashes.
See LICENSE file in root folder
*/
#pragma once

#include <Ashes/Pipeline/DepthStencilState.hpp>

namespace vk_renderer
{
	/**
	*\brief
	*	Convertit un ashes::DepthStencilState en VkPipelineDepthStencilStateCreateInfo.
	*\param[in] state
	*	Le ashes::DepthStencilState.
	*\return
	*	Le VkPipelineDepthStencilStateCreateInfo.
	*/
	VkPipelineDepthStencilStateCreateInfo convert( ashes::DepthStencilState const & state );
}
