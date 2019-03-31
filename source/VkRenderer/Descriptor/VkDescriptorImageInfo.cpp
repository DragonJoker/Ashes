#include "VkRendererPrerequisites.hpp"

#include "Buffer/VkBuffer.hpp"
#include "Image/VkSampler.hpp"
#include "Image/VkImageView.hpp"

namespace vk_renderer
{
	VkDescriptorImageInfo convert( ashes::DescriptorImageInfo const & value )
	{
		VkDescriptorImageInfo result{};
		result.imageLayout = convert( value.imageLayout );

		if ( bool( value.imageView ) )
		{
			result.imageView = static_cast< ImageView const & >( value.imageView.value().get() );
		}

		if ( bool( value.sampler ) )
		{
			result.sampler = static_cast< Sampler const & >( value.sampler.value().get() );
		}

		return result;
	}
}
