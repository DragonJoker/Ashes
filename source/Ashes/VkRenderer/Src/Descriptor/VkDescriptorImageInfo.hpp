/*
This file belongs to Ashes.
See LICENSE file in root folder
*/
#pragma once

#include <Descriptor/DescriptorImageInfo.hpp>

namespace vk_renderer
{
	/**
	*\brief
	*	Convertit un ashes::DescriptorImageInfo en VkDescriptorImageInfo.
	*\param[in] mode
	*	Le ashes::DescriptorImageInfo.
	*\return
	*	Le VkDescriptorImageInfo.
	*/
	VkDescriptorImageInfo convert( ashes::DescriptorImageInfo const & value );
}
