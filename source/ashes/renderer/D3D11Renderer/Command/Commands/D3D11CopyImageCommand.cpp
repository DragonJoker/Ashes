/*
This file belongs to Ashes.
See LICENSE file in root folder.
*/
#include "Command/Commands/D3D11CopyImageCommand.hpp"

#include "Image/D3D11Image.hpp"
#include "Image/D3D11ImageView.hpp"

#include "ashesd3d11_api.hpp"

namespace ashes::d3d11
{
	namespace
	{
		D3D11_BOX doGetSrcBox( VkImageCopy const & copyInfo )
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

	CopyImageCommand::CopyImageCommand( VkDevice device
		, VkImageCopy const & copyInfo
		, VkImage src
		, VkImage dst )
		: CommandBase{ device }
		, m_src{ src }
		, m_dst{ dst }
		, m_copyInfo{ copyInfo }
		, m_srcBox{ doGetSrcBox( m_copyInfo ) }
		, m_srcSubresource{ D3D11CalcSubresource( m_copyInfo.srcSubresource.mipLevel
			, m_copyInfo.srcSubresource.baseArrayLayer
			, get( m_src )->getMipmapLevels() ) }
		, m_dstSubresource{ D3D11CalcSubresource( m_copyInfo.dstSubresource.mipLevel
			, m_copyInfo.dstSubresource.baseArrayLayer
			, get( m_dst )->getMipmapLevels() ) }
	{
	}

	void CopyImageCommand::apply( Context const & context )const
	{
		if ( isDepthOrStencilFormat( get( m_src )->getFormat() ) )
		{
			context.context->CopySubresourceRegion( get( m_dst )->getResource()
				, m_dstSubresource
				, 0
				, 0
				, 0
				, get( m_src )->getResource()
				, m_srcSubresource
				, nullptr );
		}
		else
		{
			context.context->CopySubresourceRegion( get( m_dst )->getResource()
				, m_dstSubresource
				, UINT( m_copyInfo.dstOffset.x )
				, UINT( m_copyInfo.dstOffset.y )
				, UINT( m_copyInfo.dstOffset.z )
				, get( m_src )->getResource()
				, m_srcSubresource
				, &m_srcBox );
		}

		auto dstMemory = get( m_dst )->getMemory();
		get( dstMemory )->updateDownload( 0u, get( m_dst )->getMemoryRequirements().size, 0u );
	}

	CommandPtr CopyImageCommand::clone()const
	{
		return std::make_unique< CopyImageCommand >( *this );
	}
}
