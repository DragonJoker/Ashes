/*
This file belongs to Ashes.
See LICENSE file in root folder
*/
#pragma once

#include <Ashes/AshesPrerequisites.hpp>

namespace vk_renderer
{
	/**
	*\brief
	*	Convertit un ashes::DynamicStateFlags en VkPipelineDynamicStateEnableCreateFlags.
	*\param[in] flags
	*	Le ashes::DynamicStateFlags.
	*\return
	*	Le VkPipelineDynamicStateEnableCreateFlags.
	*/
	VkPipelineDynamicStateEnableCreateFlags convert( ashes::DynamicStateFlags const & flags );
}
