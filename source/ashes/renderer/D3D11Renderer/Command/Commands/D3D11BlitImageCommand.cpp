/*
This file belongs to GlInstance.
See LICENSE file in root folder.
*/
#include "Command/Commands/D3D11BlitImageCommand.hpp"

#include "Core/D3D11Device.hpp"
#include "Image/D3D11Image.hpp"
#include "Image/D3D11ImageView.hpp"
#include "RenderPass/D3D11FrameBuffer.hpp"

#include <Ashes/Image/ImageSubresourceRange.hpp>
#include <Ashes/RenderPass/FrameBufferAttachment.hpp>

namespace ashes::d3d11
{
	BlitImageCommand::Attachment::Attachment( ashes::ImageSubresourceLayers & subresource
		, Image const & image
		, uint32_t layer )
		: image{ image.getResource() }
		, subResourceIndex{ D3D11CalcSubresource( subresource.mipLevel, layer, image.getMipmapLevels() ) }
	{
	}

	BlitImageCommand::LayerCopy::LayerCopy( ashes::ImageBlit blitRegion
		, Image const & srcImage
		, Image const & dstImage
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

	BlitImageCommand::BlitImageCommand( VkDevice device
		, VkImage srcImage
		, VkImage dstImage
		, std::vector< ashes::ImageBlit > const & regions
		, VkFilter filter )
		: CommandBase{ device }
		, m_srcTexture{ static_cast< Image const & >( srcImage ) }
		, m_dstTexture{ static_cast< Image const & >( dstImage ) }
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
