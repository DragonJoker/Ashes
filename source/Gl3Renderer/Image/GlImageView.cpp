#include "Image/GlImageView.hpp"

#include "Core/GlDevice.hpp"
#include "Image/GlImage.hpp"
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
					assert( "Unsupported ImageViewType for a multisampled texture." );
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
		, m_target{ convert( m_createInfo, texture.getCreateFlags(), texture.getSamplesCount() ) }
	{
	}

	ImageView::~ImageView()
	{
	}
}
