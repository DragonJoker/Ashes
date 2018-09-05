/*
This file belongs to GlRenderer.
See LICENSE file in root folder.
*/
#include "GlCopySubImageCommand.hpp"

#include "Image/GlTexture.hpp"
#include "Image/GlTextureView.hpp"

#include <Image/ImageSubresourceRange.hpp>

namespace gl_renderer
{
	CopySubImageCommand::CopySubImageCommand( Device const & device
		, renderer::ImageCopy const & copyInfo
		, renderer::Texture const & src
		, renderer::Texture const & dst )
		: CommandBase{ device }
		, m_copyInfo{ copyInfo }
		, m_src{ static_cast< Texture const & >( src ) }
		, m_dst{ static_cast< Texture const & >( dst ) }
		, m_srcInternal{ getInternal( m_src.getFormat() ) }
		, m_srcFormat{ getFormat( m_srcInternal ) }
		, m_srcType{ getType( m_srcInternal ) }
		, m_srcTarget{ convert( m_src.getType(), m_src.getLayerCount(), m_src.getFlags() ) }
		, m_dstInternal{ getInternal( m_dst.getFormat() ) }
		, m_dstFormat{ getFormat( m_dstInternal ) }
		, m_dstType{ getType( m_dstInternal ) }
		, m_dstTarget{ convert( m_dst.getType(), m_dst.getLayerCount(), m_src.getFlags() ) }
	{
	}

	void CopySubImageCommand::apply( ContextLock const & context )const
	{
		glLogCommand( "CopySubImageCommand" );
		glLogCall( context
			, glBindTexture
			, m_srcTarget
			, m_src.getImage() );
		glLogCall( context
			, glBindTexture
			, m_dstTarget
			, m_dst.getImage() );
		glLogCall( context
			, glCopyImageSubData_ARB
			, m_src.getImage()
			, m_srcTarget
			, m_copyInfo.srcSubresource.mipLevel
			, m_copyInfo.srcOffset.x
			, m_copyInfo.srcOffset.y
			, m_copyInfo.srcSubresource.baseArrayLayer ? int32_t( m_copyInfo.srcSubresource.baseArrayLayer ) : m_copyInfo.srcOffset.z
			, m_dst.getImage()
			, m_dstTarget
			, m_copyInfo.dstSubresource.mipLevel
			, m_copyInfo.dstOffset.x
			, m_copyInfo.dstOffset.y
			, m_copyInfo.dstSubresource.baseArrayLayer ? int32_t( m_copyInfo.dstSubresource.baseArrayLayer ) : m_copyInfo.dstOffset.z
			, m_copyInfo.extent.width
			, m_copyInfo.extent.height
			, m_copyInfo.extent.depth );
		glLogCall( context
			, glBindTexture
			, m_dstTarget
			, 0u );
		glLogCall( context
			, glBindTexture
			, m_srcTarget
			, 0u );
		static_cast< renderer::Texture const & >( m_dst ).generateMipmaps();
	}

	CommandPtr CopySubImageCommand::clone()const
	{
		return std::make_unique< CopySubImageCommand >( *this );
	}
}
