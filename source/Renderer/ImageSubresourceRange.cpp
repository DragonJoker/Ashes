#include "ImageSubresourceRange.hpp"

namespace renderer
{
	ImageSubresourceRange::ImageSubresourceRange( ImageAspectFlags aspectMask
		, uint32_t baseMipLevel
		, uint32_t levelCount
		, uint32_t baseArrayLayer
		, uint32_t layerCount )
		: m_range
		{
			convert( aspectMask ),
			baseMipLevel,
			levelCount,
			baseArrayLayer,
			layerCount
		}
	{
	}
}
