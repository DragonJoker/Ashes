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
		, VkImageViewCreateInfo createInfo
		, bool createView )
		: m_device{ device }
		, m_createInfo{ std::move( createInfo ) }
		, m_gltextureType{ gl3::convert( getType()
			, get( m_createInfo.image )->getType()
			, get( m_createInfo.image )->getCreateFlags()
			, get( m_createInfo.image )->getSamples()
			, m_createInfo.subresourceRange.baseArrayLayer ) }
		, m_glviewType{ gl4::convertViewType( getType()
			, get( m_createInfo.image )->getType()
			, m_createInfo.subresourceRange.layerCount
			, get( m_createInfo.image )->getSamples() ) }
	{
		auto image = get( m_createInfo.image );
		auto & range = m_createInfo.subresourceRange;

		if ( range.levelCount == RemainingArrayLayers )
		{
			range.levelCount = ashes::getMaxMipCount( image->getDimensions() );
		}

		if ( range.layerCount == RemainingArrayLayers )
		{
			range.layerCount = get( device )->getLimits().maxImageArrayLayers;
		}

		if ( createView )
		{
			if ( image->getType() == VK_IMAGE_TYPE_3D )
			{
				m_createInfo.viewType = VK_IMAGE_VIEW_TYPE_3D;
				m_createInfo.subresourceRange.baseArrayLayer = 0u;
				m_createInfo.subresourceRange.layerCount = 1u;
			}

			// Non initialised textures come from back buffers, ignore them
			if ( image->hasInternal()
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
					, image->getInternal()
					, getInternalFormat( getFormat() )
					, getSubresourceRange().baseMipLevel
					, getSubresourceRange().levelCount
					, getSubresourceRange().baseArrayLayer
					, getSubresourceRange().layerCount );
				glLogCall( context
					, glBindTexture
					, GlTextureType( m_glviewType )
					, m_internal );

				if ( getComponents().r != VK_COMPONENT_SWIZZLE_IDENTITY
					&& getComponents().r != VK_COMPONENT_SWIZZLE_R )
				{
					glLogCall( context
						, glTexParameteri
						, GlTextureType( m_glviewType )
						, GL_SWIZZLE_R
						, convertComponentSwizzle( getComponents().r ) );
				}

				if ( getComponents().g != VK_COMPONENT_SWIZZLE_IDENTITY
					&& getComponents().g != VK_COMPONENT_SWIZZLE_G )
				{
					glLogCall( context
						, glTexParameteri
						, GlTextureType( m_glviewType )
						, GL_SWIZZLE_G
						, convertComponentSwizzle( getComponents().g ) );
				}

				if ( getComponents().b != VK_COMPONENT_SWIZZLE_IDENTITY
					&& getComponents().b != VK_COMPONENT_SWIZZLE_B )
				{
					glLogCall( context
						, glTexParameteri
						, GlTextureType( m_glviewType )
						, GL_SWIZZLE_B
						, convertComponentSwizzle( getComponents().b ) );
				}

				if ( getComponents().a != VK_COMPONENT_SWIZZLE_IDENTITY
					&& getComponents().a != VK_COMPONENT_SWIZZLE_A )
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
				if ( minLevel != getSubresourceRange().baseMipLevel )
				{
					reportWarning( get( this )
						, VK_SUCCESS
						, "ImageViewStatus"
						, "Image base mip level doesn't match requested one." );
				}

				int numLevels = 0;
				glLogCall( context
					, glGetTexParameteriv
					, GlTextureType( m_glviewType )
					, GL_TEX_PARAMETER_VIEW_NUM_LEVELS
					, &numLevels );
				if ( numLevels != getSubresourceRange().levelCount )
				{
					reportWarning( get( this )
						, VK_SUCCESS
						, "ImageViewStatus"
						, "Image mip level count doesn't match requested one." );
				}

				int minLayer = 0;
				glLogCall( context
					, glGetTexParameteriv
					, GlTextureType( m_glviewType )
					, GL_TEX_PARAMETER_VIEW_MIN_LAYER
					, &minLayer );
				if ( minLayer != getSubresourceRange().baseArrayLayer )
				{
					reportWarning( get( this )
						, VK_SUCCESS
						, "ImageViewStatus"
						, "Image base array layer doesn't match requested one." );
				}

				int numLayers = 0;
				glLogCall( context
					, glGetTexParameteriv
					, GlTextureType( m_glviewType )
					, GL_TEX_PARAMETER_VIEW_NUM_LAYERS
					, &numLayers );
				if ( numLayers != getSubresourceRange().layerCount )
				{
					reportWarning( get( this )
						, VK_SUCCESS
						, "ImageViewStatus"
						, "Image array layer count doesn't match requested one." );
				}

				glLogCall( context
					, glBindTexture
					, GlTextureType( m_glviewType )
					, 0u );
			}
		}
	}

	ImageView::ImageView( VkDevice device
		, VkImage image )
		: ImageView{ device
			, { VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO
				, nullptr
				, 0u
				, image
				, VK_IMAGE_VIEW_TYPE_2D
				, get( image )->getFormat()
				, {}
				, { VK_IMAGE_ASPECT_COLOR_BIT, 0u, 1u, 0u, 1u } }
			, false }
	{
	}

	ImageView::ImageView( VkDevice device
		, VkImageViewCreateInfo createInfo )
		: ImageView{ device
			, std::move( createInfo )
			, true }
	{
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

		assert( m_createInfo.image != VK_NULL_HANDLE );
		return get( m_createInfo.image )->getInternal();
	}
}
