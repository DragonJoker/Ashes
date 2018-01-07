#include "VkTextureView.hpp"

#include "VkCommandBuffer.hpp"
#include "VkDevice.hpp"
#include "VkImageMemoryBarrier.hpp"
#include "VkImageSubresourceRange.hpp"
#include "VkRenderingResources.hpp"
#include "VkTexture.hpp"

#include <VkLib/Queue.hpp>

namespace vk_renderer
{
	TextureView::TextureView( Device const & device
		, Texture const & image
		, utils::PixelFormat format )
		: m_device{ device }
		, m_format{ convert( format ) }
		, m_image{ image }
		, m_subResourceRange
		{
			isDepthStencilFormat( m_format )    // aspectMask
				? renderer::ImageAspectFlags( renderer::ImageAspectFlag::eDepth | renderer::ImageAspectFlag::eStencil )
				: isDepthFormat( m_format )
					? renderer::ImageAspectFlags( renderer::ImageAspectFlag::eDepth )
					: isStencilFormat( m_format )
						? renderer::ImageAspectFlags( renderer::ImageAspectFlag::eStencil )
						: renderer::ImageAspectFlags( renderer::ImageAspectFlag::eColour ),
			0u,                               // baseMipLevel
			1u,                               // levelCount
			0u,                               // baseArrayLayer
			1u                                // layerCount
		}
	{
		VkImageViewCreateInfo createInfo
		{
			VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO,
			nullptr,
			0,                                          // flags
			m_image,                                    // image
			VK_IMAGE_VIEW_TYPE_2D,                      // viewType
			m_format,                                   // format
			{                                           // components
				VK_COMPONENT_SWIZZLE_IDENTITY,              // r
				VK_COMPONENT_SWIZZLE_IDENTITY,              // g
				VK_COMPONENT_SWIZZLE_IDENTITY,              // b
				VK_COMPONENT_SWIZZLE_IDENTITY               // a
			},
			convert( m_subResourceRange )
		};
		DEBUG_DUMP( createInfo );
		auto res = CreateImageView( m_device
			, &createInfo
			, nullptr
			, &m_view );

		if ( !checkError( res ) )
		{
			throw std::runtime_error{ "Image view creation failed: " + getLastError() };
		}
	}

	TextureView::TextureView( TextureView && view )noexcept
		: m_device{ view.m_device }
		, m_image{ view.m_image }
	{
		swap( *this, view );
		view.m_view = VK_NULL_HANDLE;
	}

	TextureView & TextureView::operator=( TextureView && rhs )noexcept
	{
		swap( rhs, *this );
		rhs.m_view = VK_NULL_HANDLE;
		return *this;
	}

	TextureView::~TextureView()
	{
		if ( m_view )
		{
			DestroyImageView( m_device
				, m_view
				, nullptr );
		}
	}
}
