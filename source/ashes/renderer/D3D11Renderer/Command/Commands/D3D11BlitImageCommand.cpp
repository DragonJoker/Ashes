/*
This file belongs to GlInstance.
See LICENSE file in root folder.
*/
#include "Command/Commands/D3D11BlitImageCommand.hpp"

#include "Core/D3D11Device.hpp"
#include "Image/D3D11Image.hpp"
#include "Image/D3D11ImageView.hpp"
#include "RenderPass/D3D11FrameBuffer.hpp"

#include "ashesd3d11_api.hpp"

namespace ashes::d3d11
{
	std::string const & getComputeShaderCode()
	{
		static std::string const result =
		R"(cbuffer CB : register( b0 )
{
	float2 TexelSize;   // 1.0 / destination dimension
}

[numthreads( 8, 8, 1 )]
void main( uint3 DTid : SV_DispatchThreadID )
{
	//DTid is the thread ID * the values from numthreads above and in this case correspond to the pixels location in number of pixels.
	//As a result texcoords (in 0-1 range) will point at the center between the 4 pixels used for the mipmap.
	float2 texcoords = TexelSize * (DTid.xy + 0.5);

	//The samplers linear interpolation will mix the four pixel values to the new pixels color
	float4 color = SrcTexture.SampleLevel(BilinearClamp, texcoords, 0);

	//Write the final color into the destination texture.
DstTexture[DTid.xy] = color;
})";
		return result;
	}

	BlitImageCommand::Attachment::Attachment( VkImageSubresourceLayers & subresource
		, VkImage image
		, uint32_t layer )
		: image{ get( image )->getResource() }
		, subResourceIndex{ D3D11CalcSubresource( subresource.mipLevel
			, layer
			, get( image )->getMipmapLevels() ) }
	{
	}

	BlitImageCommand::LayerCopy::LayerCopy( VkImageBlit blitRegion
		, VkImage srcImage
		, VkImage dstImage
		, uint32_t layer )
		: dstOffset{ blitRegion.dstOffsets[0] }
		, srcBox
		{
			UINT( blitRegion.srcOffsets[0].x ),
			UINT( blitRegion.srcOffsets[0].y ),
			UINT( blitRegion.srcOffsets[0].z ),
			UINT( blitRegion.srcOffsets[0].x + blitRegion.srcOffsets[1].x ),
			UINT( blitRegion.srcOffsets[0].y + blitRegion.srcOffsets[1].y ),
			UINT( blitRegion.srcOffsets[0].z + blitRegion.srcOffsets[1].z )
		}
		, src{ blitRegion.srcSubresource, srcImage, layer }
		, dst{ blitRegion.dstSubresource, dstImage, layer }
	{
	}

	BlitImageCommand::BlitImageCommand( VkDevice device
		, VkImage srcImage
		, VkImage dstImage
		, VkImageBlitArray const & regions
		, VkFilter filter )
		: CommandBase{ device }
		, m_srcTexture{ static_cast< VkImage >( srcImage ) }
		, m_dstTexture{ static_cast< VkImage >( dstImage ) }
		, m_filter{ getFilterType( filter ) }
	{
		assert( get( srcImage )->getLayerCount() == get( dstImage )->getLayerCount() );

		for ( auto & region : regions )
		{
			for ( uint32_t layer = 0u; layer < get( srcImage )->getLayerCount(); ++layer )
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
