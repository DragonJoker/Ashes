/*
This file belongs to Ashes.
See LICENSE file in root folder.
*/
#include "Core/VkBackBuffer.hpp"
#include "Core/VkDevice.hpp"

namespace vk_renderer
{
	BackBuffer::BackBuffer( Device const & device
		, ashes::TexturePtr image
		, uint32_t imageIndex
		, ashes::Format format
		, Texture const & imageRef )
		: ashes::BackBuffer{ device
			, std::move( image )
			, imageRef.createView( {
				ashes::TextureViewType( imageRef.getType() ),
				format,
				ashes::ComponentMapping{},
				{
					ashes::getAspectMask( format ),
					0u,
					1u,
					0u,
					1u,
				}
			} )
			, imageIndex }
	{
	}
}
