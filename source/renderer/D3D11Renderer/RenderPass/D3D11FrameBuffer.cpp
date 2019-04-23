/*
This file belongs to Ashes.
See LICENSE file in root folder.
*/
#include "RenderPass/D3D11FrameBuffer.hpp"

#include "Core/D3D11Device.hpp"
#include "Image/D3D11ImageView.hpp"
#include "RenderPass/D3D11RenderPass.hpp"

namespace ashes::d3d11
{
	namespace
	{
		ImageViewCRefArray convert( ashes::ImageViewPtrArray const & attachs )
		{
			ImageViewCRefArray result;
			result.reserve( attachs.size() );

			for ( auto & attach : attachs )
			{
				result.emplace_back( static_cast< ImageView const & >( attach.getView() ) );
			}

			return result;
		}

		std::vector< ID3D11View * > getAllViews( ImageViewCRefArray const & views )
		{
			std::vector< ID3D11View * > result;

			for ( auto & view : views )
			{
				if ( isDepthOrStencilFormat( view.get().getFormat() ) )
				{
					result.push_back( view.get().getDepthStencilView() );
				}
				else
				{
					result.push_back( view.get().getRenderTargetView() );
				}
			}

			return result;
		}

		std::vector< ID3D11RenderTargetView * > getRenderTargetViews( ImageViewCRefArray const & views )
		{
			std::vector< ID3D11RenderTargetView * > result;

			for ( auto & view : views )
			{
				if ( !isDepthOrStencilFormat( view.get().getFormat() ) )
				{
					result.push_back( view.get().getRenderTargetView() );
				}
			}

			return result;
		}

		ImageView const * doGetDepthStencilView( ImageViewCRefArray const & views )
		{
			ImageView const * result{ nullptr };

			for ( auto & view : views )
			{
				if ( !result
					&& isDepthOrStencilFormat( view.get().getFormat() ) )
				{
					result = &view.get();
				}
			}

			return result;
		}

		ID3D11DepthStencilView * getDepthStencilView( ImageViewCRefArray const & views )
		{
			ID3D11DepthStencilView * result{ nullptr };
			auto view = doGetDepthStencilView( views );

			if ( view )
			{
				result = view->getDepthStencilView();
			}

			return result;
		}

		UINT getDepthStencilFlags( ImageViewCRefArray const & views )
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

	Framebuffer::Framebuffer( Device const & device
		, RenderPass const & renderPass
		, VkExtent2D const & dimensions
		, ashes::ImageViewPtrArray attachments )
		: ashes::FrameBuffer{ renderPass, dimensions, std::move( attachments ) }
		, m_device{ device }
		, m_views{ convert( m_attachments ) }
		, m_dimensions{ dimensions }
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
