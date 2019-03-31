/*
This file belongs to GlInstance.
See LICENSE file in root folder.
*/
#include "Command/Commands/D3D11BeginSubpassCommand.hpp"

#include "Buffer/D3D11Buffer.hpp"
#include "Image/D3D11ImageView.hpp"
#include "RenderPass/D3D11FrameBuffer.hpp"
#include "RenderPass/D3D11RenderPass.hpp"

#include <Ashes/RenderPass/ClearValue.hpp>
#include <Ashes/RenderPass/AttachmentDescription.hpp>

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
		auto & allViews = m_frameBuffer.getAllViews();

		for ( auto & attach : m_subpass.colorAttachments )
		{
			m_attaches.push_back( reinterpret_cast< ID3D11RenderTargetView * >( allViews[attach.attachment] ) );
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

			for ( auto & write : context.uavs )
			{
				for ( auto & uav : write.write.bufferInfo )
				{
					auto & buffer = static_cast< Buffer const & >( uav.buffer.get() );
					uavs.push_back( buffer.getUnorderedAccessView() );
				}

				for ( auto & uav : write.write.imageInfo )
				{
					auto & view = static_cast< ImageView const & >( uav.imageView.value().get() );
					uavs.push_back( view.getUnorderedAccessView() );
				}
			}

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
