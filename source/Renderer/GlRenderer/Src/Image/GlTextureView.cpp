#include "Image/GlTextureView.hpp"

#include "Core/GlDevice.hpp"
#include "Image/GlTexture.hpp"
#include "Sync/GlImageMemoryBarrier.hpp"

namespace gl_renderer
{
	TextureView::TextureView( Device const & device
		, Texture const & texture
		, renderer::TextureType type
		, renderer::PixelFormat format
		, uint32_t baseMipLevel
		, uint32_t levelCount
		, uint32_t baseArrayLayer
		, uint32_t layerCount
		, renderer::ComponentMapping const & mapping )
		: renderer::TextureView{ device
			, texture
			, type
			, format
			, baseMipLevel
			, levelCount
			, baseArrayLayer
			, layerCount
			, mapping }
		, m_device{ device }
		, m_target{ convert( type ) }
	{
		glLogCall( gl::GenTextures, 1, &m_texture );
		glLogCall( gl::TextureView
			, m_texture
			, m_target
			, texture.getImage()
			, getInternal( format )
			, baseMipLevel
			, levelCount
			, baseArrayLayer
			, layerCount );
		glLogCall( gl::BindTexture, m_target, m_texture );

		if ( mapping.r != renderer::ComponentSwizzle::eIdentity )
		{
			glLogCall( gl::TexParameteri, m_target, GL_SWIZZLE_R, convert( mapping.r ) );
		}

		if ( mapping.g != renderer::ComponentSwizzle::eIdentity )
		{
			glLogCall( gl::TexParameteri, m_target, GL_SWIZZLE_G, convert( mapping.g ) );
		}

		if ( mapping.b != renderer::ComponentSwizzle::eIdentity )
		{
			glLogCall( gl::TexParameteri, m_target, GL_SWIZZLE_B, convert( mapping.b ) );
		}

		if ( mapping.a != renderer::ComponentSwizzle::eIdentity )
		{
			glLogCall( gl::TexParameteri, m_target, GL_SWIZZLE_A, convert( mapping.a ) );
		}

		glLogCall( gl::BindTexture, m_target, 0u );
	}

	TextureView::~TextureView()
	{
		glLogCall( gl::DeleteTextures, 1, &m_texture );
	}

	GLuint TextureView::getImage()const noexcept
	{
		assert( m_texture != GL_INVALID_INDEX );
		return m_texture;
	}

	renderer::ImageMemoryBarrier TextureView::doMakeLayoutTransition( renderer::ImageLayout srcLayout
		, renderer::ImageLayout dstLayout
		, renderer::AccessFlags srcAccessFlags
		, renderer::AccessFlags dstAccessMask
		, uint32_t srcQueueFamily
		, uint32_t dstQueueFamily )const
	{
		renderer::ImageMemoryBarrier transitionBarrier
		{
			srcAccessFlags,
			dstAccessMask,
			srcLayout,
			dstLayout,
			srcQueueFamily,
			dstQueueFamily,
			getTexture(),
			getSubResourceRange()
		};
		return transitionBarrier;
	}
}
