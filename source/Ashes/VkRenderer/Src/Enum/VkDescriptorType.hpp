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
	*	Convertit un ashes::DescriptorType en VkDescriptorType.
	*\param[in] type
	*	Le ashes::DescriptorType.
	*\return
	*	Le VkDescriptorType.
	*/
	VkDescriptorType convert( ashes::DescriptorType const & type );
}
