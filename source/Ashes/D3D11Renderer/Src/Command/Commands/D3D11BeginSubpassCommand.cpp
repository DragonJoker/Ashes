/*
This file belongs to GlRenderer.
See LICENSE file in root folder.
*/
#include "D3D11BeginSubpassCommand.hpp"

#include "RenderPass/D3D11FrameBuffer.hpp"
#include "RenderPass/D3D11RenderPass.hpp"

#include <RenderPass/ClearValue.hpp>
#include <RenderPass/AttachmentDescription.hpp>

namespace d3d11_renderer
{
	BeginSubpassCommand::BeginSubpassCommand( Device const & device
		, ashes::RenderPass const & renderPass
		, ashes::FrameBuffer const & frameBuffer
		, ashes::SubpassDescription const & subpass )
		: CommandBase{ device }
		, m_renderPass{ static_cast< RenderPass const & >( renderPass ) }
		, m_subpass{ subpass }
		, m_frameBuffer{ static_cast< FrameBuffer const & >( frameBuffer ) }
	{
		auto & rtViews = m_frameBuffer.getRTViews();

		for ( auto & attach : m_subpass.colorAttachments )
		{
			m_attaches.push_back( rtViews[attach.attachment] );
		}

		if ( bool( subpass.depthStencilAttachment ) )
		{
			m_depthAttach = m_frameBuffer.getDSView();
		}
	}

	void BeginSubpassCommand::apply( Context const & context )const
	{
		if ( context.uavs.empty() )
		{
			context.context->OMSetRenderTargets( UINT( m_attaches.size() )
				, m_attaches.data()
				, m_depthAttach );
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

			context.context->OMSetRenderTargetsAndUnorderedAccessViews( UINT( m_attaches.size() )
				, m_attaches.data()
				, m_depthAttach
				, UINT( m_attaches.size() )
				, UINT( uavs.size() )
				, uavs.data()
				, nullptr );
		}
	}

	CommandPtr BeginSubpassCommand::clone()const
	{
		return std::make_unique< BeginSubpassCommand >( *this );
	}
}
