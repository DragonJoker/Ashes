/*
This file belongs to GlRenderer.
See LICENSE file in root folder.
*/
#include "D3D11BlitImageCommand.hpp"

#include "Core/D3D11Device.hpp"
#include "Image/D3D11Texture.hpp"
#include "Image/D3D11TextureView.hpp"
#include "RenderPass/D3D11FrameBuffer.hpp"

#include <Image/ImageSubresourceRange.hpp>
#include <RenderPass/FrameBufferAttachment.hpp>

namespace d3d11_renderer
{
	BlitImageCommand::Attachment::Attachment( renderer::ImageSubresourceLayers & subresource
		, Texture const & image
		, uint32_t layer )
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
		, m_filter{ convert( filter ) }
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

	void BlitImageCommand::apply( Context const & context )const
	{
		for ( auto & playerCopy : m_layerCopies )
		{
			auto & layerCopy = *playerCopy;
		}
	}

	CommandPtr BlitImageCommand::clone()const
	{
		return std::make_unique< BlitImageCommand >( *this );
	}
}
