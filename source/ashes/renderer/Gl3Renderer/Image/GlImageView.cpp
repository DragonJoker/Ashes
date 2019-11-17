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
			, VkSampleCountFlagBits samples
			, uint32_t baseArrayLayer )
		{
			GlTextureType result = gl3::convert( viewType, samples );

			if ( checkFlag( flags, VK_IMAGE_CREATE_CUBE_COMPATIBLE_BIT ) )
			{
				auto layer = baseArrayLayer % 6u;
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
		, m_target{ convert( getType()
			, get( image )->getCreateFlags()
			, get( image )->getSamples()
			, 0u ) }
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
		, m_target{ convert( getType()
			, get( createInfo.image )->getCreateFlags()
			, get( createInfo.image )->getSamples()
			, m_subresourceRange.baseArrayLayer ) }
	{
	}

	ImageView::~ImageView()
	{
	}

	GLuint ImageView::getInternal()const noexcept
	{
		assert( m_image != VK_NULL_HANDLE );
		return get( m_image )->getInternal();
	}
}
