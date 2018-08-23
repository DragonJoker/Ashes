/*
This file belongs to GlRenderer.
See LICENSE file in root folder.
*/
#include "GlCopyImageToBufferCommand.hpp"

#include "Buffer/GlBuffer.hpp"
#include "Core/GlDevice.hpp"
#include "Image/GlTexture.hpp"
#include "Image/GlTextureView.hpp"

#include <Image/ImageSubresourceRange.hpp>
#include <Miscellaneous/BufferImageCopy.hpp>

namespace gl_renderer
{
	namespace
	{
		std::vector< TextureViewPtr > createViews( Device const & device
			, renderer::Texture const & texture
			, renderer::BufferImageCopyArray const & copies )
		{
			std::vector< TextureViewPtr > result;
			renderer::TextureType type = texture.getType();
			renderer::TextureViewType viewType;

			if ( type == renderer::TextureType::e3D )
			{
				viewType = renderer::TextureViewType::e3D;
			}
			else if ( type == renderer::TextureType::e2D )
			{
				viewType = renderer::TextureViewType::e2D;
			}
			else if ( type == renderer::TextureType::e1D )
			{
				viewType = renderer::TextureViewType::e1D;
			}

			for ( auto & copy : copies )
			{
				renderer::ImageViewCreateInfo createInfo{};
				createInfo.viewType = viewType;
				createInfo.format = texture.getFormat();
				createInfo.subresourceRange.aspectMask = renderer::getAspectMask( createInfo.format );
				createInfo.subresourceRange.baseArrayLayer = copy.imageSubresource.baseArrayLayer;
				createInfo.subresourceRange.layerCount = copy.imageSubresource.layerCount;
				createInfo.subresourceRange.baseMipLevel = copy.imageSubresource.mipLevel;
				createInfo.subresourceRange.levelCount = 1u;
				result.emplace_back( std::make_unique< TextureView >( device
					, static_cast< Texture const & >( texture )
					, createInfo ) );
			}

			return result;
		}
	}

	CopyImageToBufferCommand::CopyImageToBufferCommand( Device const & device
		, renderer::BufferImageCopyArray const & copyInfo
		, renderer::Texture const & src
		, renderer::BufferBase const & dst )
		: CommandBase{ device }
		, m_src{ static_cast< Texture const & >( src ) }
		, m_dst{ static_cast< Buffer const & >( dst ) }
		, m_copyInfo{ copyInfo }
		, m_internal{ getInternal( m_src.getFormat() ) }
		, m_format{ getFormat( m_internal ) }
		, m_type{ getType( m_internal ) }
		, m_target{ convert( m_src.getType(), 1u ) }
		, m_views{ createViews( m_device, m_src, m_copyInfo ) }
		, m_srcFbo{ device.getBlitSrcFbo() }
	{
	}

	CopyImageToBufferCommand::CopyImageToBufferCommand( CopyImageToBufferCommand const & rhs )
		: CommandBase{ rhs.m_device }
		, m_src{ rhs.m_src }
		, m_dst{ rhs.m_dst }
		, m_copyInfo{ rhs.m_copyInfo }
		, m_internal{ rhs.m_internal }
		, m_format{ rhs.m_format }
		, m_type{ rhs.m_type }
		, m_target{ rhs.m_target }
		, m_views{ createViews( m_device, m_src, m_copyInfo ) }
		, m_srcFbo{ rhs.m_srcFbo }
	{
	}

	void CopyImageToBufferCommand::apply()const
	{
		glLogCommand( "CopyImageToBufferCommand" );
		glLogCall( m_device.getContext()
			, glBindBuffer
			, GL_BUFFER_TARGET_PIXEL_PACK
			, m_dst.getBuffer() );

		for ( size_t i = 0; i < m_views.size(); ++i )
		{
			applyOne( m_copyInfo[i]
				, *m_views[i] );
		}

		glLogCall( m_device.getContext()
			, glBindBuffer
			, GL_BUFFER_TARGET_PIXEL_PACK
			, 0u );
	}

	void CopyImageToBufferCommand::applyOne( renderer::BufferImageCopy const & copyInfo
		, TextureView const & view )const
	{
		// Setup source FBO
		glLogCall( m_device.getContext()
			, glBindFramebuffer
			, GL_FRAMEBUFFER
			, m_srcFbo );
		glLogCall( m_device.getContext()
			, glFramebufferTexture2D
			, GL_FRAMEBUFFER
			, GL_ATTACHMENT_POINT_COLOR0
			, GL_TEXTURE_2D
			, view.getImage()
			, 0u );
		glLogCall( m_device.getContext()
			, glReadBuffer
			, GL_ATTACHMENT_POINT_COLOR0 );
		glLogCall( m_device.getContext()
			, glBindFramebuffer
			, GL_FRAMEBUFFER
			, 0u );

		// Read pixels
		glLogCall( m_device.getContext()
			, glBindFramebuffer
			, GL_READ_FRAMEBUFFER
			, m_srcFbo );
		glLogCall( m_device.getContext()
			, glReadPixels
			, copyInfo.imageOffset.x
			, copyInfo.imageOffset.y
			, copyInfo.imageExtent.width
			, copyInfo.imageExtent.height
			, m_format
			, m_type
			, nullptr );
		glLogCall( m_device.getContext()
			, glBindFramebuffer
			, GL_READ_FRAMEBUFFER
			, 0u );
	}

	CommandPtr CopyImageToBufferCommand::clone()const
	{
		return std::make_unique< CopyImageToBufferCommand >( *this );
	}
}
