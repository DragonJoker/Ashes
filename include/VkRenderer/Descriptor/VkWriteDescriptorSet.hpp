/*
This file belongs to Ashes.
See LICENSE file in root folder
*/
#pragma once

#include <Ashes/Descriptor/WriteDescriptorSet.hpp>

#include <list>

namespace vk_renderer
{
	class DescriptorSet;
	struct WriteDescriptorSet
	{
		uint32_t dstBinding;
		uint32_t dstArrayElement;
		uint32_t descriptorCount;
		VkDescriptorType descriptorType;
		std::vector< VkDescriptorImageInfo > imageInfo;
		std::vector< VkDescriptorBufferInfo > bufferInfo;
		std::vector< VkBufferView > texelBufferView;
	};
	/**
	*\brief
	*	Convertit un ashes::WriteDescriptorSet en VkWriteDescriptorSet.
	*\param[in] mode
	*	Le ashes::WriteDescriptorSet.
	*\return
	*	Le VkWriteDescriptorSet.
	*/
	WriteDescriptorSet convert( ashes::WriteDescriptorSet const & value );
}
