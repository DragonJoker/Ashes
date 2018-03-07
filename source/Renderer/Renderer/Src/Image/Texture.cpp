/*
This file belongs to RendererLib.
See LICENSE file in root folder.
*/
#include "Image/Texture.hpp"

#include "Buffer/StagingBuffer.hpp"
#include "Image/TextureView.hpp"

namespace renderer
{
	Texture::Texture( Texture && rhs )
		: m_device{ rhs.m_device }
		, m_imageType{ rhs.m_imageType }
		, m_format{ rhs.m_format }
		, m_dimensions{ rhs.m_dimensions }
		, m_mipLevels{ rhs.m_mipLevels }
		, m_arrayLayers{ rhs.m_arrayLayers }
	{
	}

	Texture & Texture::operator=( Texture && rhs )
	{
		if ( &rhs != this )
		{
			m_imageType = rhs.m_imageType;
			m_format = rhs.m_format;
			m_dimensions = rhs.m_dimensions;
			m_mipLevels = rhs.m_mipLevels;
			m_arrayLayers = rhs.m_arrayLayers;
		}

		return *this;
	}

	Texture::Texture( Device const & device
		, TextureType type
		, Format format
		, Extent3D dimensions
		, uint32_t mipLevels
		, uint32_t arrayLayers )
		: m_device{ device }
		, m_imageType{ type }
		, m_format{ format }
		, m_dimensions{ dimensions }
		, m_mipLevels{ mipLevels }
		, m_arrayLayers{ arrayLayers }
	{
	}

	TextureViewPtr Texture::createView( TextureViewType type
		, Format format
		, uint32_t baseMipLevel
		, uint32_t levelCount
		, uint32_t baseArrayLayer
		, uint32_t layerCount
		, ComponentMapping const & mapping )const
	{
		return createView(
		{
			type,
			format,
			mapping,
			{
				getAspectMask( format ),
				baseMipLevel,
				levelCount,
				baseArrayLayer,
				layerCount
			}
		} );
	}
}
