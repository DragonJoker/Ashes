#include "Image/GlImageView.hpp"

#include "Core/GlDevice.hpp"
#include "Image/GlImage.hpp"
#include "Miscellaneous/GlCallLogger.hpp"

#include "ashesgl_api.hpp"

namespace ashes::gl
{
	namespace gl3
	{
		GlTextureType convert( VkImageViewType viewType
			, VkImageType imageType
			, VkImageCreateFlags flags
			, VkSampleCountFlagBits samples
			, uint32_t baseArrayLayer )
		{
			GlTextureType result = gl3::convertViewType( viewType, imageType, samples );

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
		, m_gltextureType{ gl3::convert( getType()
			, get( image )->getType()
			, get( image )->getCreateFlags()
			, get( image )->getSamples()
			, 0u ) }
		, m_glviewType{ gl4::convertViewType( getType()
			, get( image )->getType()
			, 1u
			, get( m_image )->getSamples() ) }
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
		, m_gltextureType{ gl3::convert( getType()
			, get( createInfo.image )->getType()
			, get( createInfo.image )->getCreateFlags()
			, get( createInfo.image )->getSamples()
			, m_subresourceRange.baseArrayLayer ) }
		, m_glviewType{ gl4::convertViewType( getType()
			, get( m_image )->getType()
			, m_subresourceRange.layerCount
			, get( m_image )->getSamples() ) }
	{
		if ( get( m_image )->getType() == VK_IMAGE_TYPE_3D )
		{
			m_viewType = VK_IMAGE_VIEW_TYPE_3D;
			m_subresourceRange.baseArrayLayer = 0u;
			m_subresourceRange.layerCount = 1u;
		}

		// Non initialised textures come from back buffers, ignore them
		if ( get( createInfo.image )->hasInternal()
			&& hasTextureViews( m_device ) )
		{
			auto context = get( m_device )->getContext();
			glLogCall( context
				, glGenTextures
				, 1
				, &m_internal );
			glLogCall( context
				, glTextureView
				, m_internal
				, m_glviewType
				, get( createInfo.image )->getInternal()
				, getInternalFormat( getFormat() )
				, getSubresourceRange().baseMipLevel
				, getSubresourceRange().levelCount
				, getSubresourceRange().baseArrayLayer
				, getSubresourceRange().layerCount );
			glLogCall( context
				, glBindTexture
				, GlTextureType( m_glviewType )
				, m_internal );

			if ( getComponents().r != VK_COMPONENT_SWIZZLE_IDENTITY )
			{
				glLogCall( context
					, glTexParameteri
					, GlTextureType( m_glviewType )
					, GL_SWIZZLE_R
					, convertComponentSwizzle( getComponents().r ) );
			}

			if ( getComponents().g != VK_COMPONENT_SWIZZLE_IDENTITY )
			{
				glLogCall( context
					, glTexParameteri
					, GlTextureType( m_glviewType )
					, GL_SWIZZLE_G
					, convertComponentSwizzle( getComponents().g ) );
			}

			if ( getComponents().b != VK_COMPONENT_SWIZZLE_IDENTITY )
			{
				glLogCall( context
					, glTexParameteri
					, GlTextureType( m_glviewType )
					, GL_SWIZZLE_B
					, convertComponentSwizzle( getComponents().b ) );
			}

			if ( getComponents().a != VK_COMPONENT_SWIZZLE_IDENTITY )
			{
				glLogCall( context
					, glTexParameteri
					, GlTextureType( m_glviewType )
					, GL_SWIZZLE_A
					, convertComponentSwizzle( getComponents().a ) );
			}

			int minLevel = 0;
			glLogCall( context
				, glGetTexParameteriv
				, GlTextureType( m_glviewType )
				, GL_TEX_PARAMETER_VIEW_MIN_LEVEL
				, &minLevel );
			assert( minLevel == getSubresourceRange().baseMipLevel );
			int numLevels = 0;
			glLogCall( context
				, glGetTexParameteriv
				, GlTextureType( m_glviewType )
				, GL_TEX_PARAMETER_VIEW_NUM_LEVELS
				, &numLevels );
			assert( numLevels == getSubresourceRange().levelCount );
			int minLayer = 0;
			glLogCall( context
				, glGetTexParameteriv
				, GlTextureType( m_glviewType )
				, GL_TEX_PARAMETER_VIEW_MIN_LAYER
				, &minLayer );
			assert( minLayer == getSubresourceRange().baseArrayLayer );
			int numLayers = 0;
			glLogCall( context
				, glGetTexParameteriv
				, GlTextureType( m_glviewType )
				, GL_TEX_PARAMETER_VIEW_NUM_LAYERS
				, &numLayers );
			assert( numLayers == getSubresourceRange().layerCount );
			glLogCall( context
				, glBindTexture
				, GlTextureType( m_glviewType )
				, 0u );
		}
	}

	ImageView::~ImageView()
	{
		if ( hasTextureViews( m_device ) )
		{
			auto context = get( m_device )->getContext();
			glLogCall( context
				, glDeleteTextures
				, 1
				, &m_internal );
		}
	}

	GLuint ImageView::getInternal()const noexcept
	{
		if ( m_internal != GL_INVALID_INDEX )
		{
			return m_internal;
		}

		assert( m_image != VK_NULL_HANDLE );
		return get( m_image )->getInternal();
	}
}
