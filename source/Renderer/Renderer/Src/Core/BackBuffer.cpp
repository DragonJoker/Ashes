/*
This file belongs to RendererLib.
See LICENSE file in root folder.
*/
#include "Core/BackBuffer.hpp"

#include "Core/Device.hpp"
#include "Core/SwapChain.hpp"
#include "Image/Texture.hpp"

namespace renderer
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
