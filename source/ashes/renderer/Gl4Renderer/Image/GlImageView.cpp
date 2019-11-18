#include "Image/GlImageView.hpp"

#include "Core/GlDevice.hpp"
#include "Image/GlImage.hpp"
#include "Miscellaneous/GlCallLogger.hpp"

#include "ashesgl4_api.hpp"

namespace ashes::gl4
{
	ImageView::ImageView( VkDevice device
		, VkImage image )
		: m_device{ device }
		, m_flags{ 0u }
		, m_image{ image }
		, m_viewType{ VK_IMAGE_VIEW_TYPE_2D }
		, m_format{ get( image )->getFormat() }
		, m_components{}
		, m_subresourceRange{ VK_IMAGE_ASPECT_COLOR_BIT, 0u, 1u, 0u, 1u }
		, m_target{ convertViewType( getType(), 1u, get( image )->getSamples() ) }
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
		, m_target{ convertViewType( getType(), m_subresourceRange.layerCount, get( createInfo.image )->getSamples() ) }
	{
		// Non initialised textures come from back buffers, ignore them
		if ( get( createInfo.image )->hasInternal() )
		{
			auto context = get( m_device )->getContext();
			glLogCall( context
				, glGenTextures
				, 1
				, &m_texture );
			glLogCall( context
				, glTextureView
				, m_texture
				, m_target
				, get( createInfo.image )->getInternal()
				, getInternalFormat( getFormat() )
				, getSubresourceRange().baseMipLevel
				, getSubresourceRange().levelCount
				, getSubresourceRange().baseArrayLayer
				, getSubresourceRange().layerCount );
			glLogCall( context
				, glBindTexture
				, m_target
				, m_texture );

			if ( getComponents().r != VK_COMPONENT_SWIZZLE_IDENTITY )
			{
				glLogCall( context
					, glTexParameteri
					, m_target
					, GL_SWIZZLE_R
					, convertComponentSwizzle( getComponents().r ) );
			}

			if ( getComponents().g != VK_COMPONENT_SWIZZLE_IDENTITY )
			{
				glLogCall( context
					, glTexParameteri
					, m_target
					, GL_SWIZZLE_G
					, convertComponentSwizzle( getComponents().g ) );
			}

			if ( getComponents().b != VK_COMPONENT_SWIZZLE_IDENTITY )
			{
				glLogCall( context
					, glTexParameteri
					, m_target
					, GL_SWIZZLE_B
					, convertComponentSwizzle( getComponents().b ) );
			}

			if ( getComponents().a != VK_COMPONENT_SWIZZLE_IDENTITY )
			{
				glLogCall( context
					, glTexParameteri
					, m_target
					, GL_SWIZZLE_A
					, convertComponentSwizzle( getComponents().a ) );
			}

			int minLevel = 0;
			context->glGetTexParameteriv( m_target, GL_TEXTURE_VIEW_MIN_LEVEL, &minLevel );
			assert( minLevel == getSubresourceRange().baseMipLevel );
			int numLevels = 0;
			context->glGetTexParameteriv( m_target, GL_TEXTURE_VIEW_NUM_LEVELS, &numLevels );
			assert( numLevels == getSubresourceRange().levelCount );
			int minLayer = 0;
			context->glGetTexParameteriv( m_target, GL_TEXTURE_VIEW_MIN_LAYER, &minLayer );
			assert( minLayer == getSubresourceRange().baseArrayLayer );
			int numLayers = 0;
			context->glGetTexParameteriv( m_target, GL_TEXTURE_VIEW_NUM_LAYERS, &numLayers );
			assert( numLayers == getSubresourceRange().layerCount );
			glLogCall( context
				, glBindTexture
				, m_target
				, 0u );
		}
	}

	ImageView::~ImageView()
	{
		auto context = get( m_device )->getContext();
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
