/*
This file belongs to GlRenderer.
See LICENSE file in root folder.
*/
#include "GlCopyImageCommand.hpp"

#include "Image/GlTexture.hpp"
#include "Image/GlTextureView.hpp"

#include <Image/ImageSubresourceRange.hpp>

namespace gl_renderer
{
	CopyImageCommand::CopyImageCommand( Device const & device
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
		, m_srcTarget{ convert( m_src.getType(), m_src.getLayerCount() ) }
		, m_dstInternal{ getInternal( m_dst.getFormat() ) }
		, m_dstFormat{ getFormat( m_dstInternal ) }
		, m_dstType{ getType( m_dstInternal ) }
		, m_dstTarget{ convert( m_dst.getType(), m_dst.getLayerCount() ) }
	{
	}

	void CopyImageCommand::apply()const
	{
		glLogCommand( "CopyImageCommand" );
		glLogCall( m_device.getContext()
			, glBindTexture
			, m_srcTarget
			, m_src.getImage() );
		glLogCall( m_device.getContext()
			, glBindTexture
			, m_dstTarget
			, m_dst.getImage() );
		glLogCall( m_device.getContext()
			, glCopyImageSubData
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
		glLogCall( m_device.getContext()
			, glBindTexture
			, m_dstTarget
			, 0u );
		glLogCall( m_device.getContext()
			, glBindTexture
			, m_srcTarget
			, 0u );
		m_dst.generateMipmaps();
	}

	CommandPtr CopyImageCommand::clone()const
	{
		return std::make_unique< CopyImageCommand >( *this );
	}
}
