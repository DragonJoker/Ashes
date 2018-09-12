/*
This file belongs to GlRenderer.
See LICENSE file in root folder.
*/
#include "D3D11BeginRenderPassCommand.hpp"

#include "Buffer/D3D11BufferView.hpp"
#include "Core/D3D11Device.hpp"
#include "RenderPass/D3D11FrameBuffer.hpp"
#include "RenderPass/D3D11RenderPass.hpp"

#include <RenderPass/ClearValue.hpp>

namespace d3d11_renderer
{
	BeginRenderPassCommand::BeginRenderPassCommand( Device const & device
		, renderer::RenderPass const & renderPass
		, renderer::FrameBuffer const & frameBuffer
		, renderer::ClearValueArray const & clearValues
		, renderer::SubpassContents contents
		, renderer::SubpassDescription const & subpass )
		: CommandBase{ device }
		, m_renderPass{ static_cast< RenderPass const & >( renderPass ) }
		, m_subpass{ subpass }
		, m_frameBuffer{ static_cast< FrameBuffer const & >( frameBuffer ) }
		, m_scissor{ makeScissor( m_frameBuffer.getDimensions() ) }
	{
		for ( auto & clearValue : clearValues )
		{
			if ( clearValue.isColour() )
			{
				m_rtClearValues.push_back( clearValue );
			}
			else
			{
				m_dsClearValue = clearValue;
			}
		}
	}

	void BeginRenderPassCommand::apply( Context const & context )const
	{
		context.context->RSSetScissorRects( 1u, &m_scissor );
		auto & rtViews = m_frameBuffer.getRTViews();
		auto dsView = m_frameBuffer.getDSView();

		for ( auto i = 0u; i < rtViews.size(); ++i )
		{
			context.context->ClearRenderTargetView( rtViews[i]
				, m_rtClearValues[i].colour().float32.data() );
		}

		if ( dsView )
		{
			context.context->ClearDepthStencilView( dsView
				, m_frameBuffer.getDSViewFlags()
				, m_dsClearValue.depthStencil().depth
				, m_dsClearValue.depthStencil().stencil );
		}

		if ( context.uavs.empty() )
		{
			context.context->OMSetRenderTargets( UINT( rtViews.size() )
				, rtViews.data()
				, dsView );
		}
		else
		{
			std::vector< ID3D11UnorderedAccessView * > uavs;

			//for ( auto & write : context.uavs )
			//{
			//	for ( auto & uav : write.write.texelBufferView )
			//	{
			//		auto & view = static_cast< BufferView const & >( uav.get() );
			//		uavs.push_back( view.getView() );
			//	}
			//}

			context.context->OMSetRenderTargetsAndUnorderedAccessViews( UINT( rtViews.size() )
				, rtViews.data()
				, dsView
				, UINT( rtViews.size() )
				, UINT( uavs.size() )
				, uavs.data()
				, nullptr );
		}
	}

	CommandPtr BeginRenderPassCommand::clone()const
	{
		return std::make_unique< BeginRenderPassCommand >( *this );
	}
}
