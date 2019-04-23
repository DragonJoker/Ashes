#include "Image/GlImageView.hpp"

#include "Core/GlDevice.hpp"
#include "Image/GlImage.hpp"
#include "Sync/GlImageMemoryBarrier.hpp"

namespace gl_renderer
{
	namespace
	{
		GlTextureType convert( ashes::ImageViewCreateInfo const & createInfo
			, VkImageCreateFlags flags
			, VkSampleCountFlagBits samples )
		{
			GlTextureType result = gl_renderer::convert( createInfo.viewType );

			if ( samples > VK_SAMPLE_COUNT_1_BIT )
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

			if ( checkFlag( flags, VK_IMAGE_CREATE_CUBE_COMPATIBLE_BIT ) )
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
			VK_IMAGE_VIEW_TYPE_2D,
			image.getFormat(),
			ashes::ComponentMapping{},
			{
				VK_IMAGE_ASPECT_COLOR_BIT,
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
