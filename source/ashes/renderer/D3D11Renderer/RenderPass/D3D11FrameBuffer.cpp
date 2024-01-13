/*
This file belongs to Ashes.
See LICENSE file in root folder.
*/
#include "RenderPass/D3D11FrameBuffer.hpp"

#include "Core/D3D11Device.hpp"
#include "Image/D3D11ImageView.hpp"
#include "RenderPass/D3D11RenderPass.hpp"

#include "ashesd3d11_api.hpp"

namespace ashes::d3d11
{
	namespace
	{
		std::vector< FboAttachPtr > getAllAttaches( VkImageViewArray const & views )
		{
			std::vector< FboAttachPtr > result;

			for ( auto & view : views )
			{
				auto apiview = get( view );

				if ( isDepthOrStencilFormat( apiview->getFormat() ) )
				{
					result.emplace_back( std::make_unique< FboAttach >( view
							, apiview->getDepthStencilView()
							, get( apiview->getImage() )->getResource()
							, apiview->getSubresource( apiview->getSubResourceRange().baseArrayLayer ) ) );
				}
				else
				{
					result.emplace_back( std::make_unique< FboAttach >( view
						, apiview->getRenderTargetView()
						, get( apiview->getImage() )->getResource()
						, apiview->getSubresource( apiview->getSubResourceRange().baseArrayLayer ) ) );
				}
			}

			return result;
		}

		std::vector< FboAttach * > getRenderTargetViews( std::vector< FboAttachPtr > const & attaches
			, bool msaa )
		{
			std::vector< FboAttach * > result;

			for ( auto & attach : attaches )
			{
				auto apiview = get( attach->imageView );

				if ( !isDepthOrStencilFormat( apiview->getFormat() ) )
				{
					auto apiimage = get( apiview->getImage() );
					auto multisampled = apiimage->getSamplesCount() > VK_SAMPLE_COUNT_1_BIT;

					if ( ( msaa && multisampled )
						|| ( !msaa && !multisampled ) )
					{
						result.push_back( attach.get() );
					}
				}
			}

			return result;
		}

		FboAttach * doGetDepthStencilAttach( std::vector< FboAttachPtr > const & attaches
			, bool msaa )
		{
			FboAttach * result{ nullptr };

			for ( auto & attach : attaches )
			{
				auto apiview = get( attach->imageView );

				if ( !result
					&& isDepthOrStencilFormat( apiview->getFormat() ) )
				{
					auto multisampled = get( apiview->getImage() )->getSamplesCount() > VK_SAMPLE_COUNT_1_BIT;

					if ( ( msaa && multisampled )
						|| ( !msaa && !multisampled ) )
					{
						result = attach.get();
					}
				}
			}

			return result;
		}

		FboAttach * getDepthStencilView( std::vector< FboAttachPtr > const & attaches
			, bool msaa )
		{
			FboAttach * result{ nullptr };

			if ( auto attach = doGetDepthStencilAttach( attaches, msaa ) )
			{
				auto view = get( attach->imageView );
				auto multisampled = get( view->getImage() )->getSamplesCount() > VK_SAMPLE_COUNT_1_BIT;

				if ( ( msaa && multisampled )
					|| ( !msaa && !multisampled ) )
				{
					result = attach;
				}
			}

			return result;
		}

		UINT getDepthStencilFlags( std::vector< FboAttachPtr > const & attaches )
		{
			UINT result{ 0u };
			auto attach = doGetDepthStencilAttach( attaches, false );

			if ( !attach )
			{
				attach = doGetDepthStencilAttach( attaches, true );
			}

			if ( attach )
			{
				auto view = get( attach->imageView );
				result = ( isDepthStencilFormat( view->getFormat() )
						? D3D11_CLEAR_STENCIL | D3D11_CLEAR_DEPTH
						: 0u )
					| ( isDepthFormat( view->getFormat() )
						? D3D11_CLEAR_DEPTH
						: 0u )
					| ( isStencilFormat( view->getFormat() )
						? D3D11_CLEAR_STENCIL
						: 0u );
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
		, m_allViews{ getAllAttaches( m_views ) }
		, m_rtViews{ getRenderTargetViews( m_allViews, false ) }
		, m_msRtViews{ getRenderTargetViews( m_allViews, true ) }
		, m_dsView{ getDepthStencilView( m_allViews, false ) }
		, m_msDsView{ getDepthStencilView( m_allViews, true ) }
		, m_dsViewFlags{ getDepthStencilFlags( m_allViews ) }
		, m_multisampled{ ( !m_msRtViews.empty() ) || ( m_msDsView != nullptr ) }
	{
	}
}
