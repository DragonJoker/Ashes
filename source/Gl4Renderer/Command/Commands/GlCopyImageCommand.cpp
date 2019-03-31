/*
This file belongs to GlInstance.
See LICENSE file in root folder.
*/
#include "Command/Commands/GlCopyImageCommand.hpp"

#include "Image/GlImage.hpp"
#include "Image/GlImageView.hpp"

#include <Ashes/Image/ImageSubresourceRange.hpp>

namespace gl_renderer
{
	CopyImageCommand::CopyImageCommand( Device const & device
		, ashes::ImageCopy const & copyInfo
		, ashes::Image const & src
		, ashes::Image const & dst )
		: CommandBase{ device }
		, m_copyInfo{ copyInfo }
		, m_src{ static_cast< Image const & >( src ) }
		, m_dst{ static_cast< Image const & >( dst ) }
		, m_srcInternal{ getInternalFormat( m_src.getFormat() ) }
		, m_srcFormat{ getFormat( m_srcInternal ) }
		, m_srcType{ getType( m_srcInternal ) }
		, m_srcTarget{ convert( m_src.getType(), m_src.getLayerCount() ) }
		, m_dstInternal{ getInternalFormat( m_dst.getFormat() ) }
		, m_dstFormat{ getFormat( m_dstInternal ) }
		, m_dstType{ getType( m_dstInternal ) }
		, m_dstTarget{ convert( m_dst.getType(), m_dst.getLayerCount() ) }
	{
	}

	void CopyImageCommand::apply( ContextLock const & context )const
	{
		glLogCommand( "CopyImageCommand" );
		glLogCall( context
			, glBindTexture
			, m_srcTarget
			, m_src.getInternal() );
		glLogCall( context
			, glBindTexture
			, m_dstTarget
			, m_dst.getInternal() );
		glLogCall( context
			, glCopyImageSubData
			, m_src.getInternal()
			, m_srcTarget
			, m_copyInfo.srcSubresource.mipLevel
			, m_copyInfo.srcOffset.x
			, m_copyInfo.srcOffset.y
			, m_copyInfo.srcSubresource.baseArrayLayer ? int32_t( m_copyInfo.srcSubresource.baseArrayLayer ) : m_copyInfo.srcOffset.z
			, m_dst.getInternal()
			, m_dstTarget
			, m_copyInfo.dstSubresource.mipLevel
			, m_copyInfo.dstOffset.x
			, m_copyInfo.dstOffset.y
			, m_copyInfo.dstSubresource.baseArrayLayer ? int32_t( m_copyInfo.dstSubresource.baseArrayLayer ) : m_copyInfo.dstOffset.z
			, m_copyInfo.extent.width
			, m_copyInfo.extent.height
			, m_copyInfo.extent.depth );
		glLogCall( context
			, glGenerateMipmap
			, m_dstTarget );
		glLogCall( context
			, glBindTexture
			, m_dstTarget
			, 0u );
		glLogCall( context
			, glBindTexture
			, m_srcTarget
			, 0u );
	}

	CommandPtr CopyImageCommand::clone()const
	{
		return std::make_unique< CopyImageCommand >( *this );
	}
}
