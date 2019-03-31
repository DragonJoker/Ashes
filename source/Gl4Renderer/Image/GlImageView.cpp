#include "Image/GlImageView.hpp"

#include "Core/GlDevice.hpp"
#include "Image/GlImage.hpp"
#include "Sync/GlImageMemoryBarrier.hpp"

namespace gl_renderer
{
	namespace
	{
		GlTextureViewType convert( ashes::ImageViewType viewType
			, ashes::SampleCountFlag samples )
		{
			GlTextureViewType result = gl_renderer::convert( viewType );

			if ( samples > ashes::SampleCountFlag::e1 )
			{
				switch ( result )
				{
				case GL_TEXTURE_VIEW_2D:
					result = GL_TEXTURE_VIEW_2D_MULTISAMPLE;
					break;
				case GL_TEXTURE_VIEW_2D_ARRAY:
					result = GL_TEXTURE_VIEW_2D_MULTISAMPLE_ARRAY;
					break;
				default:
					assert( "Unsupported ImageViewType for a multisampled texture." );
					break;
				}
			}

			return result;
		}
	}

	ImageView::ImageView( Device const & device
		, Image const & image )
		: ashes::ImageView{ device
		, image
		, {
			ashes::ImageViewType::e2D,
			image.getFormat(),
			ashes::ComponentMapping{},
			{
				ashes::ImageAspectFlag::eColour,
				0u,
				1u,
				0u,
				1u
			}
		} }
		, m_device{ device }
	{
	}

	ImageView::ImageView( Device const & device
		, Image const & texture
		, ashes::ImageViewCreateInfo const & createInfo )
		: ashes::ImageView{ device
			, texture
			, createInfo }
		, m_device{ device }
		, m_target{ convert( m_createInfo.viewType, texture.getSamplesCount() ) }
	{
		// Non initialised textures come from back buffers, ignore them
		if ( texture.hasInternal() )
		{
			auto context = m_device.getContext();
			glLogCall( context
				, glGenTextures
				, 1
				, &m_texture );
			glLogCall( context
				, glTextureView
				, m_texture
				, m_target
				, texture.getInternal()
				, getInternalFormat( m_createInfo.format )
				, m_createInfo.subresourceRange.baseMipLevel
				, m_createInfo.subresourceRange.levelCount
				, m_createInfo.subresourceRange.baseArrayLayer
				, m_createInfo.subresourceRange.layerCount );
			glLogCall( context
				, glBindTexture
				, m_target
				, m_texture );

			if ( m_createInfo.components.r != ashes::ComponentSwizzle::eIdentity )
			{
				glLogCall( context
					, glTexParameteri
					, m_target
					, GL_SWIZZLE_R
					, convert( m_createInfo.components.r ) );
			}

			if ( m_createInfo.components.g != ashes::ComponentSwizzle::eIdentity )
			{
				glLogCall( context
					, glTexParameteri
					, m_target
					, GL_SWIZZLE_G
					, convert( m_createInfo.components.g ) );
			}

			if ( m_createInfo.components.b != ashes::ComponentSwizzle::eIdentity )
			{
				glLogCall( context
					, glTexParameteri
					, m_target
					, GL_SWIZZLE_B
					, convert( m_createInfo.components.b ) );
			}

			if ( m_createInfo.components.a != ashes::ComponentSwizzle::eIdentity )
			{
				glLogCall( context
					, glTexParameteri
					, m_target
					, GL_SWIZZLE_A
					, convert( m_createInfo.components.a ) );
			}

			int minLevel = 0;
			context->glGetTexParameteriv( m_target, GL_TEXTURE_VIEW_MIN_LEVEL, &minLevel );
			assert( minLevel == m_createInfo.subresourceRange.baseMipLevel );
			int numLevels = 0;
			context->glGetTexParameteriv( m_target, GL_TEXTURE_VIEW_NUM_LEVELS, &numLevels );
			assert( numLevels == m_createInfo.subresourceRange.levelCount );
			int minLayer = 0;
			context->glGetTexParameteriv( m_target, GL_TEXTURE_VIEW_MIN_LAYER, &minLayer );
			assert( minLayer == m_createInfo.subresourceRange.baseArrayLayer );
			int numLayers = 0;
			context->glGetTexParameteriv( m_target, GL_TEXTURE_VIEW_NUM_LAYERS, &numLayers );
			assert( numLayers == m_createInfo.subresourceRange.layerCount );
			glLogCall( context
				, glBindTexture
				, m_target
				, 0u );
		}
	}

	ImageView::~ImageView()
	{
		auto context = m_device.getContext();
		glLogCall( context
			, glDeleteTextures
			, 1
			, &m_texture );
	}

	GLuint ImageView::getInternal()const noexcept
	{
		assert( m_texture != GL_INVALID_INDEX );
		return m_texture;
	}
}
