#include "Image/GlImageView.hpp"

#include "Core/GlDevice.hpp"
#include "Image/GlImage.hpp"
#include "Miscellaneous/GlCallLogger.hpp"

#include "ashesgl3_api.hpp"

namespace ashes::gl3
{
	namespace
	{
		GlTextureType convert( VkImageViewType viewType
			, VkImageCreateFlags flags
			, VkSampleCountFlagBits samples )
		{
			GlTextureType result = convertViewType( viewType );

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
					assert( "Unsupported ImageViewType for a multisampled image" );
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

	ImageView::ImageView( VkDevice device
		, VkImage image )
		: m_device{ device }
		, m_flags{ 0u }
		, m_image{ image }
		, m_viewType{ VK_IMAGE_VIEW_TYPE_2D }
		, m_format{ get( image )->getFormat() }
		, m_components{}
		, m_subresourceRange{ VK_IMAGE_ASPECT_COLOR_BIT, 0u, 1u, 0u, 1u }
		, m_target{ convert( getType(), get( image )->getCreateFlags(), get( image )->getSamples() ) }
	{
	}

	ImageView::ImageView( VkDevice device
		, VkImageViewCreateInfo createInfo )
		: m_device{ device }
		, m_flags{ createInfo.flags }
		, m_image{ createInfo.image }
		, m_viewType{ createInfo.viewType }
		, m_format{ createInfo.format }
		, m_components{ createInfo.components }
		, m_subresourceRange{ createInfo.subresourceRange }
		, m_target{ convert( getType(), get( image )->getCreateFlags(), get( image )->getSamples() ) }
	{
	}

	ImageView::~ImageView()
	{
	}
}
