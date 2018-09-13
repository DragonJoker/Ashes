/*
This file belongs to Ashes.
See LICENSE file in root folder
*/
#pragma once

#include <AshesPrerequisites.hpp>

namespace vk_renderer
{
	/**
	*\brief
	*	Convertit un ashes::DescriptorPoolCreateFlags en VkDescriptorPoolCreateFlags.
	*\param[in] flags
	*	Le ashes::DescriptorPoolCreateFlags.
	*\return
	*	Le VkDescriptorPoolCreateFlags.
	*/
	VkDescriptorPoolCreateFlags convert( ashes::DescriptorPoolCreateFlags const & flags );
}
