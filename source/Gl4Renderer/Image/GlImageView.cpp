#include "Image/GlImageView.hpp"

#include "Core/GlDevice.hpp"
#include "Image/GlImage.hpp"
#include "Sync/GlImageMemoryBarrier.hpp"

namespace gl_renderer
{
	namespace
	{
		GlTextureViewType convert( VkImageViewType viewType
			, VkSampleCountFlagBits samples )
		{
			GlTextureViewType result = convertViewType( viewType );

			if ( samples > VK_SAMPLE_COUNT_1_BIT )
			{
				switch ( result )
				{
				case GL_TEXTURE_VIEW_2D:
					result = GL_TEXTURE_VIEW_2D_MULTISAMPLE;
					break;
				case GL_TEXTURE_VIEW_2D_ARRAY:
					result = GL_TEXTURE_VIEW_2D_MULTISAMPLE_ARRAY;
					break;
				default:
					assert( "Unsupported ImageViewType for a multisampled texture." );
					break;
				}
			}

			return result;
		}
	}

	ImageView::ImageView( Device const & device
		, Image const & image )
		: m_device{ device }
		, createInfo{
			VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO,
			nullptr,
			0u,
			VK_NULL_HANDLE,
			VK_IMAGE_VIEW_TYPE_2D,
			image.createInfo.format,
			VkComponentMapping{},
			{
				VK_IMAGE_ASPECT_COLOR_BIT,
				0u,
				1u,
				0u,
				1u
			}
		}
		, m_target{ convert( createInfo.viewType, image.createInfo.samples ) }
	{
	}

	ImageView::ImageView( Device const & device
		, Image const & texture
		, VkImageViewCreateInfo const & createInfo )
		: m_device{ device }
		, createInfo{ createInfo }
		, m_target{ convert( createInfo.viewType, texture.createInfo.samples ) }
	{
		// Non initialised textures come from back buffers, ignore them
		if ( texture.hasInternal() )
		{
			auto context = m_device.getContext();
			glLogCall( context
				, glGenTextures
				, 1
				, &m_texture );
			glLogCall( context
				, glTextureView
				, m_texture
				, m_target
				, texture.getInternal()
				, getInternalFormat( createInfo.format )
				, createInfo.subresourceRange.baseMipLevel
				, createInfo.subresourceRange.levelCount
				, createInfo.subresourceRange.baseArrayLayer
				, createInfo.subresourceRange.layerCount );
			glLogCall( context
				, glBindTexture
				, m_target
				, m_texture );

			if ( createInfo.components.r != VK_COMPONENT_SWIZZLE_IDENTITY )
			{
				glLogCall( context
					, glTexParameteri
					, m_target
					, GL_SWIZZLE_R
					, convertComponentSwizzle( createInfo.components.r ) );
			}

			if ( createInfo.components.g != VK_COMPONENT_SWIZZLE_IDENTITY )
			{
				glLogCall( context
					, glTexParameteri
					, m_target
					, GL_SWIZZLE_G
					, convertComponentSwizzle( createInfo.components.g ) );
			}

			if ( createInfo.components.b != VK_COMPONENT_SWIZZLE_IDENTITY )
			{
				glLogCall( context
					, glTexParameteri
					, m_target
					, GL_SWIZZLE_B
					, convertComponentSwizzle( createInfo.components.b ) );
			}

			if ( createInfo.components.a != VK_COMPONENT_SWIZZLE_IDENTITY )
			{
				glLogCall( context
					, glTexParameteri
					, m_target
					, GL_SWIZZLE_A
					, convertComponentSwizzle( createInfo.components.a ) );
			}

			int minLevel = 0;
			context->glGetTexParameteriv( m_target, GL_TEXTURE_VIEW_MIN_LEVEL, &minLevel );
			assert( minLevel == createInfo.subresourceRange.baseMipLevel );
			int numLevels = 0;
			context->glGetTexParameteriv( m_target, GL_TEXTURE_VIEW_NUM_LEVELS, &numLevels );
			assert( numLevels == createInfo.subresourceRange.levelCount );
			int minLayer = 0;
			context->glGetTexParameteriv( m_target, GL_TEXTURE_VIEW_MIN_LAYER, &minLayer );
			assert( minLayer == createInfo.subresourceRange.baseArrayLayer );
			int numLayers = 0;
			context->glGetTexParameteriv( m_target, GL_TEXTURE_VIEW_NUM_LAYERS, &numLayers );
			assert( numLayers == createInfo.subresourceRange.layerCount );
			glLogCall( context
				, glBindTexture
				, m_target
				, 0u );
		}
	}

	ImageView::~ImageView()
	{
		auto context = m_device.getContext();
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
