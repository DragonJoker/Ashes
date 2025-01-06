/*
This file belongs to Ashes.
See LICENSE file in root folder.
*/
#include "Command/Commands/XBoxEndSubpassCommand.hpp"

#include "Command/XBoxCommandBuffer.hpp"
#include "RenderPass/XBoxFrameBuffer.hpp"

#include "ashesxbox_api.hpp"

namespace ashes::xbox
{
	EndSubpassCommand::EndSubpassCommand( VkDevice device
		, VkFramebuffer frameBuffer
		, VkSubpassDescription const & subpass )
		: CommandBase{ device }
		, m_frameBuffer{ frameBuffer }
		, m_subpass{ subpass }
	{
		m_attaches.resize( m_subpass.colorAttachmentCount );
	}

	void EndSubpassCommand::apply( Context const & context )const
	{
		context.context->OMSetRenderTargetsAndUnorderedAccessViews( UINT( m_attaches.size() )
			, m_attaches.data()
			, nullptr
			, UINT( m_attaches.size() )
			, D3D11_KEEP_UNORDERED_ACCESS_VIEWS
			, nullptr
			, nullptr );

		if ( m_subpass.pResolveAttachments )
		{
			uint32_t index = 0u;

			for ( auto & resolveAttach : makeArrayView( m_subpass.pResolveAttachments, m_subpass.colorAttachmentCount ) )
			{
				auto & srcAttach = get( m_frameBuffer )->getMsRTViews()[index];
				++index;
				auto & dstAttach = get( m_frameBuffer )->getAllViews()[resolveAttach.attachment];

				context.context->ResolveSubresource( dstAttach->resource
					, dstAttach->subresource
					, srcAttach->resource
					, srcAttach->subresource
					, getDxgiFormat( get( srcAttach->imageView )->getFormat() ) );
			}
		}
	}

	CommandPtr EndSubpassCommand::clone()const
	{
		return std::make_unique< EndSubpassCommand >( *this );
	}
}
