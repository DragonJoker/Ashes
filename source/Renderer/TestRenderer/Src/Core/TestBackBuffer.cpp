/*
This file belongs to RendererLib.
See LICENSE file in root folder.
*/
#include "TestBackBuffer.hpp"
#include "Core/TestDevice.hpp"

namespace test_renderer
{
	BackBuffer::BackBuffer( Device const & device
		, renderer::TexturePtr && image
		, uint32_t imageIndex
		, renderer::Format format
		, Texture const & imageRef )
		: renderer::BackBuffer{ device
			, std::move( image )
			, imageRef.createView( {
				renderer::TextureViewType( imageRef.getType() ),
				format,
				renderer::ComponentMapping{},
				{
					renderer::getAspectMask( format ),
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
