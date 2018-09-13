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
		, ashes::RenderPass const & renderPass
		, ashes::FrameBuffer const & frameBuffer
		, ashes::ClearValueArray const & clearValues )
		: CommandBase{ device }
		, m_renderPass{ static_cast< RenderPass const & >( renderPass ) }
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
	}

	CommandPtr BeginRenderPassCommand::clone()const
	{
		return std::make_unique< BeginRenderPassCommand >( *this );
	}
}
