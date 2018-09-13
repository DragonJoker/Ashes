/*
This file belongs to Ashes.
See LICENSE file in root folder
*/
#pragma once

#include <Pipeline/MultisampleState.hpp>

namespace vk_renderer
{
	/**
	*\brief
	*	Convertit un ashes::MultisampleState en VkPipelineMultisampleStateCreateInfo.
	*\param[in] state
	*	Le ashes::MultisampleState.
	*\return
	*	Le VkPipelineMultisampleStateCreateInfo.
	*/
	VkPipelineMultisampleStateCreateInfo convert( ashes::MultisampleState const & state );
}
