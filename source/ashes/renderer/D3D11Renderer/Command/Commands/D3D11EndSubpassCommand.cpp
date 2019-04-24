/*
This file belongs to GlInstance.
See LICENSE file in root folder.
*/
#include "Command/Commands/D3D11EndSubpassCommand.hpp"

#include "Command/D3D11CommandBuffer.hpp"
#include "RenderPass/D3D11FrameBuffer.hpp"

#include <Ashes/RenderPass/ClearValue.hpp>
#include <Ashes/RenderPass/AttachmentDescription.hpp>
#include <Ashes/RenderPass/SubpassDescription.hpp>

namespace ashes::d3d11
{
	EndSubpassCommand::EndSubpassCommand( VkDevice device
		, ashes::FrameBuffer const & frameBuffer
		, ashes::SubpassDescription const & subpass )
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
				for ( auto & uav : write.write.bufferInfo )
				{
					uavs.push_back( nullptr );
				}

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
