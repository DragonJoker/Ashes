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
		void doClear( renderer::ClearAttachment const & clearAttach )
		{
			if ( renderer::checkFlag( clearAttach.aspectMask, renderer::ImageAspectFlag::eColour ) )
			{
				assert( clearAttach.clearValue.isColour() );
				auto & colour = clearAttach.clearValue.colour();
			}
			else
			{
				assert( !clearAttach.clearValue.isColour() );
				auto & depthStencil = clearAttach.clearValue.depthStencil();
				auto stencil = depthStencil.stencil;

				if ( renderer::checkFlag( clearAttach.aspectMask, renderer::ImageAspectFlag::eDepth | renderer::ImageAspectFlag::eStencil ) )
				{
				}
				else if ( renderer::checkFlag( clearAttach.aspectMask, renderer::ImageAspectFlag::eDepth ) )
				{
				}
				else if ( renderer::checkFlag( clearAttach.aspectMask, renderer::ImageAspectFlag::eStencil ) )
				{
				}
			}
		}

	}

	ClearAttachmentsCommand::ClearAttachmentsCommand( Device const & device
		, renderer::ClearAttachmentArray const & clearAttaches
		, renderer::ClearRectArray const & clearRects )
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
				doClear( clearAttach );
			}
		}
	}

	CommandPtr ClearAttachmentsCommand::clone()const
	{
		return std::make_unique< ClearAttachmentsCommand >( *this );
	}
}
