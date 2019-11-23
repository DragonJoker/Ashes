/*
This file belongs to GlInstance.
See LICENSE file in root folder.
*/
#include "Command/Commands/D3D11ResolveImageCommand.hpp"

#include "Image/D3D11Image.hpp"

#include "ashesd3d11_api.hpp"

namespace ashes::d3d11
{
	ResolveImageCommand::ResolveImageCommand( VkDevice device
		, VkImage srcImage
		, VkImage dstImage
		, VkImageResolveArray regions )
		: CommandBase{ device }
		, m_srcResource{ get( srcImage )->getResource() }
		, m_dstResource{ get( dstImage )->getResource() }
		, m_format{ getDxgiFormat( get( srcImage )->getFormat() ) }
	{
		for ( auto & region : regions )
		{
			for ( auto layer = 0u; layer < region.dstSubresource.layerCount; ++layer )
			{
				m_layers.push_back(
					{
						D3D11CalcSubresource( region.srcSubresource.mipLevel
							, region.srcSubresource.baseArrayLayer + layer
							, get( srcImage )->getMipmapLevels() ),
						D3D11CalcSubresource( region.dstSubresource.mipLevel
							, region.dstSubresource.baseArrayLayer + layer
							, get( dstImage )->getMipmapLevels() ),
					} );
			}
		}
	}

	void ResolveImageCommand::apply( Context const & context )const
	{
		for ( auto & layer : m_layers )
		{
			context.context->ResolveSubresource( m_dstResource
				, layer.dstSubresource
				, m_srcResource
				, layer.srcSubresource
				, m_format );
		}
	}

	CommandPtr ResolveImageCommand::clone()const
	{
		return std::make_unique< ResolveImageCommand >( *this );
	}
}
