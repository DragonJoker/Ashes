/*
This file belongs to Ashes.
See LICENSE file in root folder.
*/
#include "RenderPass/TestFrameBuffer.hpp"

#include "Core/TestDevice.hpp"
#include "Image/TestImageView.hpp"
#include "RenderPass/TestRenderPass.hpp"

#include "ashestest_api.hpp"

namespace ashes::test
{
	namespace
	{
		VkImageViewArray getRenderTargetViews( VkImageViewArray const & attaches
			, bool msaa )
		{
			VkImageViewArray result;

			for ( auto & attach : attaches )
			{
				if ( auto apiview = get( attach );
					!isDepthOrStencilFormat( apiview->getFormat() ) )
				{
					if ( auto multisampled = get( apiview->getImage() )->getSamplesCount() > VK_SAMPLE_COUNT_1_BIT;
						( msaa && multisampled ) || ( !msaa && !multisampled ) )
					{
						result.push_back( attach );
					}
				}
			}

			return result;
		}

		VkImageView doGetDepthStencilAttach( VkImageViewArray const & attaches
			, bool msaa )
		{
			VkImageView result{};

			for ( auto & attach : attaches )
			{
				if ( auto apiview = get( attach );
					!result && isDepthOrStencilFormat( apiview->getFormat() ) )
				{
					if ( auto multisampled = get( apiview->getImage() )->getSamplesCount() > VK_SAMPLE_COUNT_1_BIT;
						( msaa && multisampled ) || ( !msaa && !multisampled ) )
					{
						result = attach;
					}
				}
			}

			return result;
		}

		VkImageView getDepthStencilView( VkImageViewArray const & attaches
			, bool msaa )
		{
			VkImageView result{};

			if ( auto attach = doGetDepthStencilAttach( attaches, msaa ) )
			{
				if ( auto multisampled = get( get( attach )->getImage() )->getSamplesCount() > VK_SAMPLE_COUNT_1_BIT;
					( msaa && multisampled ) || ( !msaa && !multisampled ) )
				{
					result = attach;
				}
			}

			return result;
		}
	}

	Framebuffer::Framebuffer( VkDevice device
		, VkFramebufferCreateInfo createInfo )
		: m_device{ device }
		, m_createInfo{ std::move( createInfo ) }
		, m_views{ m_createInfo.pAttachments, m_createInfo.pAttachments + m_createInfo.attachmentCount }
		, m_dimensions{ m_createInfo.width, m_createInfo.height }
		, m_rtViews{ getRenderTargetViews( m_views, false ) }
		, m_msRtViews{ getRenderTargetViews( m_views, true ) }
		, m_dsView{ getDepthStencilView( m_views, false ) }
		, m_msDsView{ getDepthStencilView( m_views, true ) }
		, m_multisampled{ ( !m_msRtViews.empty() ) || ( m_msDsView != nullptr ) }
	{
	}
}
