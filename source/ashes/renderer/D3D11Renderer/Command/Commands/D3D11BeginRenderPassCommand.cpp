/*
This file belongs to GlInstance.
See LICENSE file in root folder.
*/
#include "Command/Commands/D3D11BeginRenderPassCommand.hpp"

#include "Buffer/D3D11BufferView.hpp"
#include "Core/D3D11Device.hpp"
#include "RenderPass/D3D11FrameBuffer.hpp"
#include "RenderPass/D3D11RenderPass.hpp"

#include "ashesd3d11_api.hpp"

namespace ashes::d3d11
{
	BeginRenderPassCommand::BeginRenderPassCommand( VkDevice device
		, VkRenderPass renderPass
		, VkFramebuffer frameBuffer
		, VkClearValueArray const & clearValues )
		: CommandBase{ device }
		, m_renderPass{ renderPass }
		, m_frameBuffer{ frameBuffer }
		, m_scissor{ makeScissor( get( m_frameBuffer )->getDimensions() ) }
	{
		auto rp = get( renderPass );
		assert( clearValues.size() == rp->size() );
		for ( auto & attach : *rp )
		{
			auto & clearValue = clearValues[attach.attachment];

			if ( ashes::isDepthOrStencilFormat( rp->getAttachment( attach ).format ) )
			{
				m_dsClearValue = clearValue;
			}
			else
			{
				m_rtClearValues.push_back( clearValue );
			}
		}
	}

	void BeginRenderPassCommand::apply( Context const & context )const
	{
		context.context->RSSetScissorRects( 1u, &m_scissor );
		auto & views = get( m_frameBuffer )->getAllViews();
		uint32_t clearIndex = 0u;

		for ( auto & reference : *get( m_renderPass ) )
		{
			auto & attachDesc = get( m_renderPass )->getAttachment( reference );

			if ( attachDesc.loadOp == VK_ATTACHMENT_LOAD_OP_CLEAR )
			{
				if ( getAspectMask( attachDesc.format ) == VK_IMAGE_ASPECT_COLOR_BIT )
				{
					context.context->ClearRenderTargetView( reinterpret_cast< ID3D11RenderTargetView * >( views[reference.attachment]->view )
						, m_rtClearValues[clearIndex].color.float32 );
					++clearIndex;
				}
				else
				{
					context.context->ClearDepthStencilView( reinterpret_cast< ID3D11DepthStencilView * >( views[reference.attachment]->view )
						, get( m_frameBuffer )->getDSViewFlags()
						, m_dsClearValue.depthStencil.depth
						, m_dsClearValue.depthStencil.stencil );
				}
			}
		}
	}

	CommandPtr BeginRenderPassCommand::clone()const
	{
		return std::make_unique< BeginRenderPassCommand >( *this );
	}
}
