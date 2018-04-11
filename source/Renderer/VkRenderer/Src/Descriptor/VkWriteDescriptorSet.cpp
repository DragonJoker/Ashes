#include "VkRendererPrerequisites.hpp"

#include "Buffer/VkBufferView.hpp"
#include "Descriptor/VkDescriptorSet.hpp"

namespace vk_renderer
{
	WriteDescriptorSet convert( renderer::WriteDescriptorSet const & value )
	{
		WriteDescriptorSet result;
		result.dstBinding = value.dstBinding;
		result.dstArrayElement = value.dstArrayElement;
		result.descriptorCount = value.descriptorCount;
		result.descriptorType = convert( value.descriptorType );
		result.imageInfo = convert< VkDescriptorImageInfo >( value.imageInfo );
		result.bufferInfo = convert< VkDescriptorBufferInfo >( value.bufferInfo );

		for ( auto & bufferView : value.texelBufferView )
		{
			auto & texelBufferView = static_cast< BufferView const & >( bufferView.get() );
			result.texelBufferView.push_back( static_cast< VkBufferView const & >( texelBufferView ) );
		}

		return result;
	}
}
