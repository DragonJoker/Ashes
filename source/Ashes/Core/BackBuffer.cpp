/*
This file belongs to Ashes.
See LICENSE file in root folder.
*/
#include "Ashes/Core/BackBuffer.hpp"

#include "Ashes/Core/Device.hpp"
#include "Ashes/Core/SwapChain.hpp"
#include "Ashes/Image/Texture.hpp"

namespace ashes
{
	BackBuffer::BackBuffer( Device const & device
		, TexturePtr && image
		, TextureViewPtr && view
		, uint32_t imageIndex )
		: m_device{ device }
		, m_image{ std::move( image ) }
		, m_view{ std::move( view ) }
		, m_imageIndex{ imageIndex }
	{
		registerObject( m_device, "BackBuffer", this );
	}

	BackBuffer::~BackBuffer()
	{
		unregisterObject( m_device, this );
	}
}
