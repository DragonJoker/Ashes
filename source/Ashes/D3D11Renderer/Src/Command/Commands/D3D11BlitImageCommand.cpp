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
	BlitImageCommand::Attachment::Attachment( ashes::ImageSubresourceLayers & subresource
		, Texture const & image
		, uint32_t layer )
		: image{ image.getResource() }
		, subResourceIndex{ D3D11CalcSubresource( subresource.mipLevel, layer, image.getMipmapLevels() ) }
	{
	}

	BlitImageCommand::LayerCopy::LayerCopy( ashes::ImageBlit blitRegion
		, Texture const & srcImage
		, Texture const & dstImage
		, uint32_t layer )
		: dstOffset{ blitRegion.dstOffset }
		, srcBox
		{
			UINT( blitRegion.srcOffset.x ),
			UINT( blitRegion.srcOffset.y ),
			UINT( blitRegion.srcOffset.z ),
			blitRegion.srcOffset.x + blitRegion.srcExtent.width,
			blitRegion.srcOffset.y + blitRegion.srcExtent.height,
			blitRegion.srcOffset.z + blitRegion.srcExtent.depth
		}
		, src{ blitRegion.srcSubresource, srcImage, layer }
		, dst{ blitRegion.dstSubresource, dstImage, layer }
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
			context.context->CopySubresourceRegion( layerCopy.dst.image
				, layerCopy.dst.subResourceIndex
				, layerCopy.dstOffset.x
				, layerCopy.dstOffset.y
				, layerCopy.dstOffset.z
				, layerCopy.src.image
				, layerCopy.src.subResourceIndex
				, &layerCopy.srcBox );
		}
	}

	CommandPtr BlitImageCommand::clone()const
	{
		return std::make_unique< BlitImageCommand >( *this );
	}
}
