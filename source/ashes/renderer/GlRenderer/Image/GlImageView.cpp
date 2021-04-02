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

	ImageView::ImageView( VkAllocationCallbacks const * allocInfo
		, VkDevice device
		, VkImageViewCreateInfo createInfo
		, bool createView )
		: m_device{ device }
		, m_createInfo{ adjustCreateInfo( device, std::move( createInfo ) ) }
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
		auto context = get( m_device )->getContext();
		m_pixelFormat = PixelFormat{ context
			, GlTextureType( m_glviewType )
			, m_createInfo.format
			, getComponents() };
		auto image = get( m_createInfo.image );
		auto & range = m_createInfo.subresourceRange;

		if ( createView )
		{
			// Non initialised textures come from back buffers, ignore them
			if ( image->hasInternal()
				&& hasTextureViews( m_device ) )
			{
				glLogCreateCall( context
					, glGenTextures
					, 1
					, &m_internal );
				glLogCall( context
					, glTextureView
					, m_internal
					, m_glviewType
					, image->getInternal()
					, getInternalFormat()
					, getSubresourceRange().baseMipLevel
					, getSubresourceRange().levelCount
					, getSubresourceRange().baseArrayLayer
					, getSubresourceRange().layerCount );
				glLogCall( context
					, glBindTexture
					, GlTextureType( m_glviewType )
					, m_internal );
				m_pixelFormat.applySwizzle( context, GlTextureType( m_glviewType ) );

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

		registerObject( m_device, *this );
	}

	ImageView::ImageView( VkAllocationCallbacks const * allocInfo
		, VkDevice device
		, VkImage image )
		: ImageView{ allocInfo
			, device
			, { VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO
				, nullptr
				, 0u
				, image
				, VK_IMAGE_VIEW_TYPE_2D
				, get( image )->getFormatVk()
				, {}
				, { VK_IMAGE_ASPECT_COLOR_BIT, 0u, 1u, 0u, 1u } }
			, false }
	{
	}

	ImageView::ImageView( VkAllocationCallbacks const * allocInfo
		, VkDevice device
		, VkImageViewCreateInfo createInfo )
		: ImageView{ allocInfo
			, device
			, std::move( createInfo )
			, true }
	{
	}

	ImageView::~ImageView()
	{
		unregisterObject( m_device, *this );

		if ( hasTextureViews( m_device ) )
		{
			auto context = get( m_device )->getContext();
			glLogCall( context
				, glDeleteTextures
				, 1
				, &m_internal );
		}
	}

	VkImageViewCreateInfo ImageView::adjustCreateInfo( VkDevice device
		, VkImageViewCreateInfo createInfo )
	{
		auto result = std::move( createInfo );
		auto image = get( result.image );
		auto & range = result.subresourceRange;

		if ( range.levelCount == RemainingArrayLayers )
		{
			range.levelCount = ashes::getMaxMipCount( image->getDimensions() );
		}

		if ( range.layerCount == RemainingArrayLayers )
		{
			range.layerCount = get( device )->getLimits().maxImageArrayLayers;
		}

		if ( get( result.image )->getType() == VK_IMAGE_TYPE_3D )
		{
			result.viewType = VK_IMAGE_VIEW_TYPE_3D;
			result.subresourceRange.baseArrayLayer = 0u;
			result.subresourceRange.layerCount = 1u;
		}

		return result;
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
