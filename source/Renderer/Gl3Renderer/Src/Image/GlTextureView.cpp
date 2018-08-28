#include "Image/GlTextureView.hpp"

#include "Core/GlDevice.hpp"
#include "Image/GlTexture.hpp"
#include "Sync/GlImageMemoryBarrier.hpp"

namespace gl_renderer
{
	namespace
	{
		GlTextureType convert( renderer::ImageViewCreateInfo const & createInfo
			, renderer::ImageCreateFlags flags
			, renderer::SampleCountFlag samples )
		{
			GlTextureType result = gl_renderer::convert( createInfo.viewType );

			if ( samples > renderer::SampleCountFlag::e1 )
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

			if ( checkFlag( flags, renderer::ImageCreateFlag::eCubeCompatible ) )
			{
				auto layer = createInfo.subresourceRange.baseArrayLayer % 6;
				result = GlTextureType( GL_TEXTURE_CUBE_POSITIVE_X + layer );
			}

			return result;
		}
	}

	TextureView::TextureView( Device const & device
		, Texture const & image )
		: renderer::TextureView{ device
		, image
		, {
			renderer::TextureViewType::e2D,
			image.getFormat(),
			renderer::ComponentMapping{},
			{
				renderer::ImageAspectFlag::eColour,
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
		, renderer::ImageViewCreateInfo const & createInfo )
		: renderer::TextureView{ device
			, texture
			, createInfo }
		, m_device{ device }
		, m_target{ convert( m_createInfo, texture.getFlags(), texture.getSamplesCount() ) }
	{
		auto target = texture.getTarget();
		glLogCall( m_device.getContext(), glBindTexture, target, texture.getImage() );

		if ( m_createInfo.components.r != renderer::ComponentSwizzle::eIdentity )
		{
			glLogCall( m_device.getContext(), glTexParameteri, target, GL_SWIZZLE_R, convert( m_createInfo.components.r ) );
		}

		if ( m_createInfo.components.g != renderer::ComponentSwizzle::eIdentity )
		{
			glLogCall( m_device.getContext(), glTexParameteri, target, GL_SWIZZLE_G, convert( m_createInfo.components.g ) );
		}

		if ( m_createInfo.components.b != renderer::ComponentSwizzle::eIdentity )
		{
			glLogCall( m_device.getContext(), glTexParameteri, target, GL_SWIZZLE_B, convert( m_createInfo.components.b ) );
		}

		if ( m_createInfo.components.a != renderer::ComponentSwizzle::eIdentity )
		{
			glLogCall( m_device.getContext(), glTexParameteri, target, GL_SWIZZLE_A, convert( m_createInfo.components.a ) );
		}

		glLogCall( m_device.getContext(), glBindTexture, target, 0u );
	}

	TextureView::~TextureView()
	{
	}
}
