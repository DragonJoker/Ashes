/*
This file belongs to Ashes.
See LICENSE file in root folder.
*/
#include "Command/Commands/D3D11ClearAttachmentsCommand.hpp"

#include "Core/D3D11Device.hpp"
#include "Image/D3D11ImageView.hpp"
#include "RenderPass/D3D11FrameBuffer.hpp"

#include "ashesd3d11_api.hpp"

namespace ashes::d3d11
{
	namespace
	{
		void doClear( Context const & context
			, ClearAttachmentView const & clearAttach )
		{
			if ( ashes::checkFlag( clearAttach.clear.aspectMask, VK_IMAGE_ASPECT_COLOR_BIT ) )
			{
				auto & colour = clearAttach.clear.clearValue.color;
				context.context->ClearRenderTargetView( reinterpret_cast< ID3D11RenderTargetView * >( clearAttach.view )
					, colour.float32 );
			}
			else
			{
				auto & depthStencil = clearAttach.clear.clearValue.depthStencil;
				UINT flags = ( ashes::checkFlag( clearAttach.clear.aspectMask, VK_IMAGE_ASPECT_DEPTH_BIT )
						? D3D11_CLEAR_DEPTH
						: 0u )
					| ( ashes::checkFlag( clearAttach.clear.aspectMask, VK_IMAGE_ASPECT_STENCIL_BIT )
						? D3D11_CLEAR_STENCIL
						: 0u );
				context.context->ClearDepthStencilView( reinterpret_cast< ID3D11DepthStencilView * >( clearAttach.view )
					, flags
					, depthStencil.depth
					, depthStencil.stencil );
			}
		}
	}

	ClearAttachmentsCommand::ClearAttachmentsCommand( VkDevice device
		, VkRenderPass renderPass
		, VkSubpassDescription const & subpass
		, VkFramebuffer framebuffer
		, ArrayView< VkClearAttachment const > const & clearAttaches
		, ArrayView< VkClearRect const > const & clearRects )
		: CommandBase{ device }
		, m_clearRects{ clearRects.begin(), clearRects.end() }
	{
		for ( auto & attach : clearAttaches )
		{
			if ( checkFlag( attach.aspectMask, VK_IMAGE_ASPECT_COLOR_BIT ) )
			{
				auto ref = subpass.pColorAttachments[attach.colorAttachment];
				m_clearViews.push_back( { attach, get( framebuffer )->getAllViews()[ref.attachment]->view } );
			}
			else if ( attach.aspectMask )
			{
				m_clearViews.push_back( { attach, get( framebuffer )->getDSView()->view } );
			}
		}
	}

	void ClearAttachmentsCommand::apply( Context const & context )const
	{
		for ( auto & clearAttach : m_clearViews )
		{
			for ( auto & rect : m_clearRects )
			{
				D3D11_RECT scissor
				{
					LONG( rect.rect.offset.x ),
					LONG( rect.rect.offset.y ),
					LONG( rect.rect.extent.width ),
					LONG( rect.rect.extent.height ),
				};
				context.context->RSSetScissorRects( 1u, &scissor );
				doClear( context, clearAttach );
			}
		}
	}

	CommandPtr ClearAttachmentsCommand::clone()const
	{
		return std::make_unique< ClearAttachmentsCommand >( *this );
	}
}
