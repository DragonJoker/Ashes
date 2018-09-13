/*
This file belongs to RendererLib.
See LICENSE file in root folder.
*/
#include "RenderPass/D3D11FrameBuffer.hpp"

#include "Core/D3D11Device.hpp"
#include "Image/D3D11TextureView.hpp"
#include "RenderPass/D3D11RenderPass.hpp"

#include <RenderPass/FrameBufferAttachment.hpp>

namespace d3d11_renderer
{
	namespace
	{
		TextureViewCRefArray convert( renderer::FrameBufferAttachmentArray const & attachs )
		{
			TextureViewCRefArray result;
			result.reserve( attachs.size() );

			for ( auto & attach : attachs )
			{
				result.emplace_back( static_cast< TextureView const & >( attach.getView() ) );
			}

			return result;
		}

		std::vector< ID3D11RenderTargetView * > getRenderTargetViews( TextureViewCRefArray const & views )
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

		TextureView const * doGetDepthStencilView( TextureViewCRefArray const & views )
		{
			TextureView const * result{ nullptr };

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

		ID3D11DepthStencilView * getDepthStencilView( TextureViewCRefArray const & views )
		{
			ID3D11DepthStencilView * result{ nullptr };
			auto view = doGetDepthStencilView( views );

			if ( view )
			{
				result = view->getDepthStencilView();
			}

			return result;
		}

		UINT getDepthStencilFlags( TextureViewCRefArray const & views )
		{
			UINT result{ 0u };
			auto view = doGetDepthStencilView( views );

			if ( view )
			{
				result = ( isDepthFormat( view->getFormat() )
						? D3D11_CLEAR_DEPTH
						: 0u )
					| ( isStencilFormat( view->getFormat() )
						? D3D11_CLEAR_STENCIL
						: 0u );
			}

			return result;
		}
	}

	FrameBuffer::FrameBuffer( Device const & device
		, RenderPass const & renderPass
		, renderer::Extent2D const & dimensions
		, renderer::FrameBufferAttachmentArray && attachments )
		: renderer::FrameBuffer{ renderPass, dimensions, std::move( attachments ) }
		, m_device{ device }
		, m_views{ convert( m_attachments ) }
		, m_dimensions{ dimensions }
		, m_rtViews{ getRenderTargetViews( m_views ) }
		, m_dsView{ getDepthStencilView( m_views ) }
		, m_dsViewFlags{ getDepthStencilFlags( m_views ) }
	{
	}

	FrameBuffer::~FrameBuffer()
	{
	}
}
