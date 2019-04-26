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
		std::vector< ID3D11View * > getAllViews( VkImageViewArray const & views )
		{
			std::vector< ID3D11View * > result;

			for ( auto & view : views )
			{
				if ( isDepthOrStencilFormat( get( view )->getFormat() ) )
				{
					result.push_back( get( view )->getDepthStencilView() );
				}
				else
				{
					result.push_back( get( view )->getRenderTargetView() );
				}
			}

			return result;
		}

		std::vector< ID3D11RenderTargetView * > getRenderTargetViews( VkImageViewArray const & views )
		{
			std::vector< ID3D11RenderTargetView * > result;

			for ( auto & view : views )
			{
				if ( !isDepthOrStencilFormat( get( view )->getFormat() ) )
				{
					result.push_back( get( view )->getRenderTargetView() );
				}
			}

			return result;
		}

		ImageView const * doGetDepthStencilView( VkImageViewArray const & views )
		{
			ImageView const * result{ nullptr };

			for ( auto & view : views )
			{
				if ( !result
					&& isDepthOrStencilFormat( get( view )->getFormat() ) )
				{
					result = get( view );
				}
			}

			return result;
		}

		ID3D11DepthStencilView * getDepthStencilView( VkImageViewArray const & views )
		{
			ID3D11DepthStencilView * result{ nullptr };
			auto view = doGetDepthStencilView( views );

			if ( view )
			{
				result = view->getDepthStencilView();
			}

			return result;
		}

		UINT getDepthStencilFlags( VkImageViewArray const & views )
		{
			UINT result{ 0u };
			auto view = doGetDepthStencilView( views );

			if ( view )
			{
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
		, m_allViews{ ashes::d3d11::getAllViews( m_views ) }
		, m_rtViews{ getRenderTargetViews( m_views ) }
		, m_dsView{ getDepthStencilView( m_views ) }
		, m_dsViewFlags{ getDepthStencilFlags( m_views ) }
	{
	}

	Framebuffer::~Framebuffer()
	{
	}
}
