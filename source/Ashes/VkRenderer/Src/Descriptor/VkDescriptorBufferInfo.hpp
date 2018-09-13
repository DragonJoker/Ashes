/*
This file belongs to Ashes.
See LICENSE file in root folder
*/
#pragma once

#include <Descriptor/DescriptorBufferInfo.hpp>

namespace vk_renderer
{
	/**
	*\brief
	*	Convertit un ashes::DescriptorBufferInfo en VkDescriptorBufferInfo.
	*\param[in] mode
	*	Le ashes::DescriptorBufferInfo.
	*\return
	*	Le VkDescriptorBufferInfo.
	*/
	VkDescriptorBufferInfo convert( ashes::DescriptorBufferInfo const & value );
}
