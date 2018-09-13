/*
This file belongs to Ashes.
See LICENSE file in root folder
*/
#ifndef ___VkRenderer_DescriptorSetLayoutBinding_HPP___
#define ___VkRenderer_DescriptorSetLayoutBinding_HPP___
#pragma once

#include <Descriptor/DescriptorSetLayoutBinding.hpp>

namespace vk_renderer
{
	/**
	*\brief
	*	Convertit un ashes::DescriptorSetLayoutBinding en VkDescriptorSetLayoutBinding.
	*\param[in] flags
	*	Le ashes::DescriptorSetLayoutBinding.
	*\return
	*	Le VkDescriptorSetLayoutBinding.
	*/
	VkDescriptorSetLayoutBinding convert( ashes::DescriptorSetLayoutBinding const & binding );
}

#endif
