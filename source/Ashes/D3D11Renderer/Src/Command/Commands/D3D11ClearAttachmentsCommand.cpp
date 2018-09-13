/*
This file belongs to GlRenderer.
See LICENSE file in root folder.
*/
#include "D3D11ClearAttachmentsCommand.hpp"

#include "Core/D3D11Device.hpp"
#include "Image/D3D11TextureView.hpp"

namespace d3d11_renderer
{
	namespace
	{
		void doClear( Context const & context
			, ashes::ClearAttachment const & clearAttach )
		{
			if ( ashes::checkFlag( clearAttach.aspectMask, ashes::ImageAspectFlag::eColour ) )
			{
				assert( clearAttach.clearValue.isColour() );
				auto & colour = clearAttach.clearValue.colour();
			}
			else
			{
				assert( !clearAttach.clearValue.isColour() );
				auto & depthStencil = clearAttach.clearValue.depthStencil();
				auto stencil = depthStencil.stencil;

				if ( ashes::checkFlag( clearAttach.aspectMask, ashes::ImageAspectFlag::eDepth | ashes::ImageAspectFlag::eStencil ) )
				{
				}
				else if ( ashes::checkFlag( clearAttach.aspectMask, ashes::ImageAspectFlag::eDepth ) )
				{
				}
				else if ( ashes::checkFlag( clearAttach.aspectMask, ashes::ImageAspectFlag::eStencil ) )
				{
				}
			}
		}

	}

	ClearAttachmentsCommand::ClearAttachmentsCommand( Device const & device
		, ashes::ClearAttachmentArray const & clearAttaches
		, ashes::ClearRectArray const & clearRects )
		: CommandBase{ device }
		, m_clearAttaches{ clearAttaches }
		, m_clearRects{ clearRects }
	{
	}

	void ClearAttachmentsCommand::apply( Context const & context )const
	{
		for ( auto & clearAttach : m_clearAttaches )
		{
			for ( auto & rect : m_clearRects )
			{
				D3D11_RECT scissor
				{
					LONG( rect.offset.x ),
					LONG( rect.offset.y ),
					LONG( rect.extent.width ),
					LONG( rect.extent.height ),
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
