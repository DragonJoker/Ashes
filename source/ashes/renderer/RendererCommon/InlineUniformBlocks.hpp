/**
*\file
*	InlineUniformBlocks.h
*\author
*	Sylvain Doremus
*/
#pragma once

#define VK_NO_PROTOTYPES
#include <ashes/ashes.hpp>

#include <memory>
#include <vector>

namespace ashes
{
	struct InlineUbo
	{
		VkBuffer buffer;
		VkDeviceMemory memory;
		VkDescriptorBufferInfo info;
	};
	using InlineUboPtr = std::unique_ptr< InlineUbo >;
	using InlineUboArray = std::vector< InlineUboPtr >;

	InlineUboPtr createInlineUbo( VkDevice device
		, VkWriteDescriptorSetInlineUniformBlockEXT const & inlineUniform
		, VkPhysicalDeviceMemoryProperties const & memoryProperties
		, PFN_vkCreateBuffer createBuffer
		, PFN_vkGetBufferMemoryRequirements getMemoryRequirements
		, PFN_vkAllocateMemory allocateMemory
		, PFN_vkBindBufferMemory bindMemory
		, PFN_vkMapMemory mapMemory
		, PFN_vkFlushMappedMemoryRanges flushMemory
		, PFN_vkUnmapMemory unmapMemory );

}
