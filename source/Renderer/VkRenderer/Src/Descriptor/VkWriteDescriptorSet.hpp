/*
This file belongs to RendererLib.
See LICENSE file in root folder
*/
#pragma once

#include <Descriptor/WriteDescriptorSet.hpp>

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
	*	Convertit un renderer::WriteDescriptorSet en VkWriteDescriptorSet.
	*\param[in] mode
	*	Le renderer::WriteDescriptorSet.
	*\return
	*	Le VkWriteDescriptorSet.
	*/
	WriteDescriptorSet convert( renderer::WriteDescriptorSet const & value );
}
