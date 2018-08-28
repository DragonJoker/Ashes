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
		GlImageAspectFlags getMask( renderer::Format format )
		{
			GlImageAspectFlags result = 0u;

			if ( renderer::isDepthFormat( format ) )
			{
				result |= GL_DEPTH_BUFFER_BIT;
			}

			if ( renderer::isStencilFormat( format ) )
			{
				result |= GL_STENCIL_BUFFER_BIT;
			}

			if ( !renderer::isDepthFormat( format )
				&& !renderer::isStencilFormat( format ) )
			{
				result |= GL_COLOR_BUFFER_BIT;
			}

			return result;
		}

		GlAttachmentPoint getAttachmentPoint( renderer::Format format )
		{
			switch ( format )
			{
			case renderer::Format::eD16_UNORM:
			case renderer::Format::eD32_SFLOAT:
				return GL_ATTACHMENT_POINT_DEPTH;

			case renderer::Format::eD24_UNORM_S8_UINT:
			case renderer::Format::eD32_SFLOAT_S8_UINT:
				return GL_ATTACHMENT_POINT_DEPTH_STENCIL;

			case renderer::Format::eS8_UINT:
				return GL_ATTACHMENT_POINT_STENCIL;

			default:
				return GL_ATTACHMENT_POINT_COLOR0;
			}
		}

		GlAttachmentType getAttachmentType( renderer::Format format )
		{
			switch ( format )
			{
			case renderer::Format::eD16_UNORM:
			case renderer::Format::eD32_SFLOAT:
				return GL_ATTACHMENT_TYPE_DEPTH;

			case renderer::Format::eD24_UNORM_S8_UINT:
			case renderer::Format::eD32_SFLOAT_S8_UINT:
				return GL_ATTACHMENT_TYPE_DEPTH_STENCIL;

			case renderer::Format::eS8_UINT:
				return GL_ATTACHMENT_TYPE_STENCIL;

			default:
				return GL_ATTACHMENT_TYPE_COLOR;
			}
		}
	}

	BlitImageCommand::Attachment::Attachment( renderer::ImageSubresourceLayers & subresource
		, Texture const & image
		, uint32_t layer )
		: object{ image.getImage() }
		, point{ getAttachmentPoint( image.getFormat() ) }
		, type{ getAttachmentType( image.getFormat() ) }
	{
		if ( image.getLayerCount() > 1u )
		{
			view = image.createView( 
			{
				renderer::TextureViewType( image.getType() ),
				image.getFormat(),
				renderer::ComponentMapping{},
				{
					subresource.aspectMask,
					subresource.mipLevel,
					1u,
					layer,
					1u
				}
			} );
			object = static_cast< TextureView const & >( *view ).getImage();
			subresource.mipLevel = 0u;
		}
	}

	BlitImageCommand::LayerCopy::LayerCopy( renderer::ImageBlit blitRegion
		, Texture const & srcImage
		, Texture const & dstImage
		, uint32_t layer )
		: region{ blitRegion }
		, src{ region.srcSubresource, srcImage, layer }
		, dst{ region.dstSubresource, dstImage, layer }
	{
	}

	BlitImageCommand::BlitImageCommand( Device const & device
		, renderer::Texture const & srcImage
		, renderer::Texture const & dstImage
		, std::vector< renderer::ImageBlit > const & regions
		, renderer::Filter filter )
		: CommandBase{ device }
		, m_srcTexture{ static_cast< Texture const & >( srcImage ) }
		, m_dstTexture{ static_cast< Texture const & >( dstImage ) }
		, m_srcFbo{ device.getBlitSrcFbo() }
		, m_dstFbo{ device.getBlitDstFbo() }
		, m_filter{ convert( filter ) }
		, m_mask{ getMask( m_srcTexture.getFormat() ) }
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

	void BlitImageCommand::apply()const
	{
		for ( auto & playerCopy : m_layerCopies )
		{
			auto & layerCopy = *playerCopy;
			// Setup source FBO
			glLogCall( m_device.getContext(), glBindFramebuffer, GL_FRAMEBUFFER, m_srcFbo );
			glLogCall( m_device.getContext(), glFramebufferTexture2D
				, GL_FRAMEBUFFER
				, layerCopy.src.point
				, GL_TEXTURE_2D
				, layerCopy.src.object
				, layerCopy.region.srcSubresource.mipLevel );

			// Setup dst FBO
			glLogCall( m_device.getContext(), glBindFramebuffer, GL_FRAMEBUFFER, m_dstFbo );
			glLogCall( m_device.getContext(), glFramebufferTexture2D
				, GL_FRAMEBUFFER
				, layerCopy.dst.point
				, GL_TEXTURE_2D
				, layerCopy.dst.object
				, layerCopy.region.dstSubresource.mipLevel );

			// Perform the blit
			glLogCall( m_device.getContext(), glBindFramebuffer, GL_READ_FRAMEBUFFER, m_srcFbo );
			glLogCall( m_device.getContext(), glBindFramebuffer, GL_DRAW_FRAMEBUFFER, m_dstFbo );
			glLogCall( m_device.getContext(), glBlitFramebuffer
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
			glLogCall( m_device.getContext(), glBindFramebuffer, GL_DRAW_FRAMEBUFFER, 0u );
			glLogCall( m_device.getContext(), glBindFramebuffer, GL_READ_FRAMEBUFFER, 0u );
		}
	}

	CommandPtr BlitImageCommand::clone()const
	{
		return std::make_unique< BlitImageCommand >( *this );
	}
}
