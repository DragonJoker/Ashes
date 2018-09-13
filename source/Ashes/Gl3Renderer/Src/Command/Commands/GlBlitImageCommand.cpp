/*
This file belongs to GlRenderer.
See LICENSE file in root folder.
*/
#include "GlBlitImageCommand.hpp"

#include "Core/GlDevice.hpp"
#include "Image/GlTexture.hpp"
#include "Image/GlTextureView.hpp"
#include "RenderPass/GlFrameBuffer.hpp"

#include <Image/ImageSubresourceRange.hpp>
#include <RenderPass/FrameBufferAttachment.hpp>

namespace gl_renderer
{
	namespace
	{
		GlImageAspectFlags getMask( ashes::Format format )
		{
			GlImageAspectFlags result = 0u;
			auto internal = getInternal( format );

			if ( isDepthFormat( internal ) )
			{
				result |= GL_DEPTH_BUFFER_BIT;
			}

			if ( isStencilFormat( internal ) )
			{
				result |= GL_STENCIL_BUFFER_BIT;
			}

			if ( !isDepthFormat( internal )
				&& !isStencilFormat( internal ) )
			{
				result |= GL_COLOR_BUFFER_BIT;
			}

			return result;
		}
	}

	BlitImageCommand::Attachment::Attachment( ashes::ImageSubresourceLayers & subresource
		, Texture const & image
		, uint32_t layer )
		: object{ image.getImage() }
		, point{ getAttachmentPoint( image.getFormat() ) }
		, type{ getAttachmentType( image.getFormat() ) }
	{
		if ( image.getLayerCount() > 1u )
		{
			subresource.mipLevel = 0u;
		}
	}

	BlitImageCommand::LayerCopy::LayerCopy( ashes::ImageBlit blitRegion
		, Texture const & srcImage
		, Texture const & dstImage
		, uint32_t layer )
		: region{ blitRegion }
		, src{ region.srcSubresource, srcImage, layer }
		, dst{ region.dstSubresource, dstImage, layer }
	{
	}

	BlitImageCommand::BlitImageCommand( Device const & device
		, ashes::Texture const & srcImage
		, ashes::Texture const & dstImage
		, std::vector< ashes::ImageBlit > const & regions
		, ashes::Filter filter )
		: CommandBase{ device }
		, m_srcTexture{ static_cast< Texture const & >( srcImage ) }
		, m_dstTexture{ static_cast< Texture const & >( dstImage ) }
		, m_srcFbo{ device.getBlitSrcFbo() }
		, m_dstFbo{ device.getBlitDstFbo() }
		, m_filter{ convert( filter ) }
		, m_mask{ getMask( m_srcTexture.getFormat() ) }
		, m_srcTarget{ checkFlag( m_srcTexture.getFlags(), ashes::ImageCreateFlag::eCubeCompatible ) ? GL_TEXTURE_CUBE_POSITIVE_X : GL_TEXTURE_2D }
		, m_dstTarget{ checkFlag( m_dstTexture.getFlags(), ashes::ImageCreateFlag::eCubeCompatible ) ? GL_TEXTURE_CUBE_POSITIVE_X : GL_TEXTURE_2D }
	{
		assert( srcImage.getLayerCount() == dstImage.getLayerCount() );

		for ( auto & region : regions )
		{
			for ( uint32_t layer = 0u; layer < srcImage.getLayerCount(); ++layer )
			{
				m_layerCopies.emplace_back( std::make_shared< BlitImageCommand::LayerCopy >( region
					, m_srcTexture
					, m_dstTexture
					, layer ) );
			}
		}
	}

	BlitImageCommand::~BlitImageCommand()
	{
	}

	void BlitImageCommand::apply( ContextLock const & context )const
	{
		for ( auto & playerCopy : m_layerCopies )
		{
			auto & layerCopy = *playerCopy;
			// Setup source FBO
			glLogCall( context
				, glBindFramebuffer
				, GL_FRAMEBUFFER
				, m_srcFbo );
			glLogCall( context
				, glFramebufferTexture2D
				, GL_FRAMEBUFFER
				, layerCopy.src.point
				, m_srcTarget
				, layerCopy.src.object
				, layerCopy.region.srcSubresource.mipLevel );
			checkCompleteness( context->glCheckFramebufferStatus( GL_FRAMEBUFFER ) );

			// Setup dst FBO
			glLogCall( context
				, glBindFramebuffer
				, GL_FRAMEBUFFER
				, m_dstFbo );
			glLogCall( context
				, glFramebufferTexture2D
				, GL_FRAMEBUFFER
				, layerCopy.dst.point
				, m_dstTarget
				, layerCopy.dst.object
				, layerCopy.region.dstSubresource.mipLevel );
			checkCompleteness( context->glCheckFramebufferStatus( GL_FRAMEBUFFER ) );

			// Perform the blit
			glLogCall( context
				, glBindFramebuffer
				, GL_READ_FRAMEBUFFER
				, m_srcFbo );
			glLogCall( context
				, glBindFramebuffer
				, GL_DRAW_FRAMEBUFFER
				, m_dstFbo );
			glLogCall( context
				, glBlitFramebuffer
				, layerCopy.region.srcOffset.x
				, layerCopy.region.srcOffset.y
				, layerCopy.region.srcExtent.width
				, layerCopy.region.srcExtent.height
				, layerCopy.region.dstOffset.x
				, layerCopy.region.dstOffset.y
				, layerCopy.region.dstExtent.width
				, layerCopy.region.dstExtent.height
				, m_mask
				, m_filter );
			glLogCall( context
				, glBindFramebuffer
				, GL_DRAW_FRAMEBUFFER
				, 0u );
			glLogCall( context
				, glBindFramebuffer
				, GL_READ_FRAMEBUFFER
				, 0u );
		}
	}

	CommandPtr BlitImageCommand::clone()const
	{
		return std::make_unique< BlitImageCommand >( *this );
	}
}
