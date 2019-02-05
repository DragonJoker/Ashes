/*
This file belongs to Ashes.
See LICENSE file in root folder
*/
#pragma once

#include <Pipeline/DynamicState.hpp>

namespace vk_renderer
{
	/**
	*\brief
	*	Convertit un ashes::DynamicState en VkPipelineDynamicStateEnableCreateInfo.
	*\param[in] state
	*	Le ashes::DynamicState.
	*\return
	*	Le VkPipelineDynamicStateEnableCreateInfo.
	*/
	VkPipelineDynamicStateEnableCreateInfo convert( ashes::DynamicState const & state
		, std::vector< VkDynamicStateEnable > & enables );
}
