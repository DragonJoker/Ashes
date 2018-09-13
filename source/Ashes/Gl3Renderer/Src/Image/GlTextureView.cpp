#include "Image/GlTextureView.hpp"

#include "Core/GlDevice.hpp"
#include "Image/GlTexture.hpp"
#include "Sync/GlImageMemoryBarrier.hpp"

namespace gl_renderer
{
	namespace
	{
		GlTextureType convert( ashes::ImageViewCreateInfo const & createInfo
			, ashes::ImageCreateFlags flags
			, ashes::SampleCountFlag samples )
		{
			GlTextureType result = gl_renderer::convert( createInfo.viewType );

			if ( samples > ashes::SampleCountFlag::e1 )
			{
				switch ( result )
				{
				case GL_TEXTURE_2D:
					result = GL_TEXTURE_2D_MULTISAMPLE;
					break;
				case GL_TEXTURE_2D_ARRAY:
					result = GL_TEXTURE_2D_MULTISAMPLE_ARRAY;
					break;
				default:
					assert( "Unsupported TextureViewType for a multisampled texture." );
					break;
				}
			}

			if ( checkFlag( flags, ashes::ImageCreateFlag::eCubeCompatible ) )
			{
				auto layer = createInfo.subresourceRange.baseArrayLayer % 6;
				result = GlTextureType( GL_TEXTURE_CUBE_POSITIVE_X + layer );
			}

			return result;
		}
	}

	TextureView::TextureView( Device const & device
		, Texture const & image )
		: ashes::TextureView{ device
		, image
		, {
			ashes::TextureViewType::e2D,
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

	TextureView::TextureView( Device const & device
		, Texture const & texture
		, ashes::ImageViewCreateInfo const & createInfo )
		: ashes::TextureView{ device
			, texture
			, createInfo }
		, m_device{ device }
		, m_target{ convert( m_createInfo, texture.getFlags(), texture.getSamplesCount() ) }
	{
		auto target = texture.getTarget();
		auto context = m_device.getContext();
		glLogCall( context
			, glBindTexture
			, target
			, texture.getImage() );

		if ( m_createInfo.components.r != ashes::ComponentSwizzle::eIdentity )
		{
			glLogCall( context
				, glTexParameteri
				, target
				, GL_SWIZZLE_R
				, convert( m_createInfo.components.r ) );
		}

		if ( m_createInfo.components.g != ashes::ComponentSwizzle::eIdentity )
		{
			glLogCall( context
				, glTexParameteri
				, target
				, GL_SWIZZLE_G
				, convert( m_createInfo.components.g ) );
		}

		if ( m_createInfo.components.b != ashes::ComponentSwizzle::eIdentity )
		{
			glLogCall( context
				, glTexParameteri
				, target
				, GL_SWIZZLE_B
				, convert( m_createInfo.components.b ) );
		}

		if ( m_createInfo.components.a != ashes::ComponentSwizzle::eIdentity )
		{
			glLogCall( context
				, glTexParameteri
				, target
				, GL_SWIZZLE_A
				, convert( m_createInfo.components.a ) );
		}

		glLogCall( context
			, glBindTexture
			, target
			, 0u );
	}

	TextureView::~TextureView()
	{
	}
}
