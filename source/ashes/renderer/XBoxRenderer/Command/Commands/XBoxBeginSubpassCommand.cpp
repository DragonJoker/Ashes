/*
This file belongs to Ashes.
See LICENSE file in root folder.
*/
#include "Command/Commands/XBoxBeginSubpassCommand.hpp"

#include "Buffer/XBoxBuffer.hpp"
#include "Image/XBoxImageView.hpp"
#include "RenderPass/XBoxFrameBuffer.hpp"
#include "RenderPass/XBoxRenderPass.hpp"

#include "ashesxbox_api.hpp"

namespace ashes::xbox
{
	BeginSubpassCommand::BeginSubpassCommand( VkDevice device
		, VkRenderPass renderPass
		, VkFramebuffer frameBuffer
		, VkSubpassDescription const & subpass )
		: CommandBase{ device }
		, m_renderPass{ renderPass }
		, m_subpass{ subpass }
		, m_frameBuffer{ frameBuffer }
	{
		auto & allViews = get( m_frameBuffer )->getAllViews();

		for ( auto const & attach : makeArrayView( m_subpass.pColorAttachments, m_subpass.colorAttachmentCount ) )
		{
			m_attaches.emplace_back( static_cast< ID3D11RenderTargetView * >( allViews[attach.attachment]->view ) );
		}

		if ( m_subpass.pDepthStencilAttachment )
		{
			auto & attach = *m_subpass.pDepthStencilAttachment;
			m_depthAttach = static_cast< ID3D11DepthStencilView * >( allViews[attach.attachment]->view );
		}
	}

	void BeginSubpassCommand::apply( Context const & context )const
	{
		auto uavStart = UINT( m_attaches.size() );
		context.context->OMSetRenderTargetsAndUnorderedAccessViews( UINT( m_attaches.size() )
			, m_attaches.data()
			, m_depthAttach
			, uavStart
			, D3D11_KEEP_UNORDERED_ACCESS_VIEWS
			, nullptr
			, nullptr );

		if ( context.uavStart != uavStart )
		{
			context.uavStart = uavStart;

			if ( !context.rawUavs.empty() )
			{
				context.context->OMSetRenderTargetsAndUnorderedAccessViews( D3D11_KEEP_RENDER_TARGETS_AND_DEPTH_STENCIL
					, nullptr
					, nullptr
					, context.uavStart
					, UINT( context.rawUavs.size() )
					, context.rawUavs.data()
					, nullptr );
			}
		}
	}

	CommandPtr BeginSubpassCommand::clone()const
	{
		return std::make_unique< BeginSubpassCommand >( *this );
	}
}
