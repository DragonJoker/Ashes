/*
This file belongs to GlInstance.
See LICENSE file in root folder.
*/
#include "Command/Commands/D3D11CopyImageCommand.hpp"

#include "Image/D3D11Texture.hpp"
#include "Image/D3D11TextureView.hpp"

#include <Ashes/Image/ImageSubresourceRange.hpp>

namespace d3d11_renderer
{
	namespace
	{
		D3D11_BOX doGetSrcBox( ashes::ImageCopy const & copyInfo )
		{
			return
			{
				UINT( copyInfo.srcOffset.x ),
				UINT( copyInfo.srcOffset.y ),
				UINT( copyInfo.srcOffset.z ),
				UINT( copyInfo.srcOffset.x ) + copyInfo.extent.width,
				UINT( copyInfo.srcOffset.y ) + copyInfo.extent.height,
				UINT( copyInfo.srcOffset.z ) + copyInfo.extent.depth,
			};
		}
	}

	CopyImageCommand::CopyImageCommand( Device const & device
		, ashes::ImageCopy const & copyInfo
		, ashes::Texture const & src
		, ashes::Texture const & dst )
		: CommandBase{ device }
		, m_copyInfo{ copyInfo }
		, m_src{ static_cast< Texture const & >( src ) }
		, m_dst{ static_cast< Texture const & >( dst ) }
		, m_srcBox{ doGetSrcBox( m_copyInfo ) }
		, m_srcSubresource{ D3D11CalcSubresource( m_copyInfo.srcSubresource.mipLevel
			, m_copyInfo.srcSubresource.baseArrayLayer
			, m_src.getMipmapLevels() ) }
		, m_dstSubresource{ D3D11CalcSubresource( m_copyInfo.dstSubresource.mipLevel
			, m_copyInfo.dstSubresource.baseArrayLayer
			, m_dst.getMipmapLevels() ) }
	{
	}

	void CopyImageCommand::apply( Context const & context )const
	{
		if ( isDepthOrStencilFormat( m_src.getFormat() ) )
		{
			context.context->CopySubresourceRegion( m_dst.getResource()
				, m_dstSubresource
				, 0
				, 0
				, 0
				, m_src.getResource()
				, m_srcSubresource
				, nullptr );
		}
		else
		{
			context.context->CopySubresourceRegion( m_dst.getResource()
				, m_dstSubresource
				, m_copyInfo.dstOffset.x
				, m_copyInfo.dstOffset.y
				, m_copyInfo.dstOffset.z
				, m_src.getResource()
				, m_srcSubresource
				, &m_srcBox );
		}
	}

	CommandPtr CopyImageCommand::clone()const
	{
		return std::make_unique< CopyImageCommand >( *this );
	}
}
