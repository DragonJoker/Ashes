/*
This file belongs to GlRenderer.
See LICENSE file in root folder.
*/
#include "D3D11EndSubpassCommand.hpp"

#include "Command/D3D11CommandBuffer.hpp"
#include "RenderPass/D3D11FrameBuffer.hpp"

#include <RenderPass/ClearValue.hpp>
#include <RenderPass/AttachmentDescription.hpp>
#include <RenderPass/SubpassDescription.hpp>

namespace d3d11_renderer
{
	EndSubpassCommand::EndSubpassCommand( Device const & device
		, renderer::FrameBuffer const & frameBuffer
		, renderer::SubpassDescription const & subpass )
		: CommandBase{ device }
		, m_frameBuffer{ static_cast< FrameBuffer const & >( frameBuffer ) }
		, m_subpass{ subpass }
	{
		assert( m_subpass.resolveAttachments.empty()
			|| m_subpass.resolveAttachments.size() == m_subpass.colorAttachments.size() );

		auto & rtViews = m_frameBuffer.getRTViews();

		for ( auto & attach : m_subpass.colorAttachments )
		{
			m_attaches.push_back( nullptr );
		}
	}

	void EndSubpassCommand::apply( Context const & context )const
	{
		std::vector< ID3D11RenderTargetView * > rtViews( m_frameBuffer.getRTViews().size(), nullptr );

		if ( context.uavs.empty() )
		{
			context.context->OMSetRenderTargets( UINT( m_attaches.size() )
				, m_attaches.data()
				, nullptr );
		}
		else
		{
			std::vector< ID3D11UnorderedAccessView * > uavs;

			for ( auto & write : context.uavs )
			{
				for ( auto & uav : write.write.texelBufferView )
				{
					uavs.push_back( nullptr );
				}
			}

			context.context->OMSetRenderTargetsAndUnorderedAccessViews( UINT( m_attaches.size() )
				, m_attaches.data()
				, nullptr
				, UINT( m_attaches.size() )
				, UINT( uavs.size() )
				, uavs.data()
				, nullptr );
		}
	}

	CommandPtr EndSubpassCommand::clone()const
	{
		return std::make_unique< EndSubpassCommand >( *this );
	}
}
