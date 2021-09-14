/**
*\file
*	InlineUniformBlocks.cpp
*\author
*	Sylvain Doremus
*/
#include "renderer/RendererCommon/InlineUniformBlocks.hpp"

#include <cstring>

namespace ashes
{
	InlineUboPtr createInlineUbo( VkDevice device
		, VkWriteDescriptorSetInlineUniformBlockEXT const & inlineUniform
		, VkPhysicalDeviceMemoryProperties const & memoryProperties
		, PFN_vkCreateBuffer createBuffer
		, PFN_vkGetBufferMemoryRequirements getMemoryRequirements
		, PFN_vkAllocateMemory allocateMemory
		, PFN_vkBindBufferMemory bindMemory
		, PFN_vkMapMemory mapMemory
		, PFN_vkFlushMappedMemoryRanges flushMemory
		, PFN_vkUnmapMemory unmapMemory )
	{
		InlineUboPtr inlineUbo = std::make_unique< InlineUbo >();
		VkBufferCreateInfo bufferInfo
		{
			VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO,
			nullptr,
			0u,
			inlineUniform.dataSize,
			VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT,
			VK_SHARING_MODE_EXCLUSIVE,
			0u,
			nullptr,
		};
		auto res = createBuffer( device
			, &bufferInfo
			, nullptr
			, &inlineUbo->buffer );

		if ( res == VK_SUCCESS )
		{
			VkMemoryRequirements requirements;
			getMemoryRequirements( device
				, inlineUbo->buffer
				, &requirements );
			auto deduced = deduceMemoryType( requirements.memoryTypeBits
				, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT
				, memoryProperties );
			VkMemoryAllocateInfo allocate
			{
				VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO,
				nullptr,
				requirements.size,
				deduced
			};
			res = allocateMemory( device
				, &allocate
				, nullptr
				, &inlineUbo->memory );

			if ( res == VK_SUCCESS )
			{
				res = bindMemory( device, inlineUbo->buffer, inlineUbo->memory, 0u );

				if ( res == VK_SUCCESS )
				{
					uint8_t * buffer{ nullptr };

					if ( mapMemory( device, inlineUbo->memory, 0, requirements.size, 0u, reinterpret_cast< void ** >( &buffer ) ) == VK_SUCCESS )
					{
						std::memcpy( buffer, inlineUniform.pData, inlineUniform.dataSize );
						VkMappedMemoryRange range
						{
							VK_STRUCTURE_TYPE_MAPPED_MEMORY_RANGE,
							nullptr,
							inlineUbo->memory,
							0u,
							requirements.size
						};
						flushMemory( device, 1u, &range );
						unmapMemory( device, inlineUbo->memory );
					}

					inlineUbo->info =
					{
						inlineUbo->buffer,
						0u,
						requirements.size,
					};
				}
			}
		}

		return inlineUbo;
	}
}
