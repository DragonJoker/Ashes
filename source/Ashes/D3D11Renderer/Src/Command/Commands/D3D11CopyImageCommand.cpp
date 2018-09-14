/*
This file belongs to GlRenderer.
See LICENSE file in root folder.
*/
#include "D3D11CopyImageCommand.hpp"

#include "Image/D3D11Texture.hpp"
#include "Image/D3D11TextureView.hpp"

#include <Image/ImageSubresourceRange.hpp>

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
				copyInfo.extent.width,
				copyInfo.extent.height,
				copyInfo.extent.depth,
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
	{
	}

	void CopyImageCommand::apply( Context const & context )const
	{
		context.context->CopySubresourceRegion( m_dst.getResource()
			, D3D11CalcSubresource( m_copyInfo.dstSubresource.mipLevel
				, m_copyInfo.dstSubresource.baseArrayLayer
				, m_dst.getMipmapLevels() )
			, m_copyInfo.dstOffset.x
			, m_copyInfo.dstOffset.y
			, m_copyInfo.dstOffset.z
			, m_src.getResource()
			, D3D11CalcSubresource( m_copyInfo.srcSubresource.mipLevel
				, m_copyInfo.srcSubresource.baseArrayLayer
				, m_dst.getMipmapLevels() )
			, &m_srcBox );
	}

	CommandPtr CopyImageCommand::clone()const
	{
		return std::make_unique< CopyImageCommand >( *this );
	}
}
