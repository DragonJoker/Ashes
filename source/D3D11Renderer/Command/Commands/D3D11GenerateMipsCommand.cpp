/*
This file belongs to GlRenderer.
See LICENSE file in root folder.
*/
#include "Command/Commands/D3D11GenerateMipsCommand.hpp"

#include "Image/D3D11Texture.hpp"

namespace d3d11_renderer
{
	namespace
	{
		ashes::ImageViewCreateInfo getCreateInfos( Texture const & texture )
		{
			ashes::ImageViewCreateInfo result;
			result.viewType = ashes::TextureViewType( texture.getType() );
			result.format = texture.getFormat();
			result.subresourceRange.aspectMask = getAspectMask( texture.getFormat() );
			result.subresourceRange.baseArrayLayer = 0u;
			result.subresourceRange.baseMipLevel = 0u;
			result.subresourceRange.layerCount = texture.getLayerCount();
			result.subresourceRange.levelCount = texture.getMipmapLevels();
			return result;
		}
	}

	GenerateMipsCommand::GenerateMipsCommand( Device const & device
		, Texture const & texture )
		: CommandBase{ device }
		, m_texture{ texture }
		, m_view{ device, texture, getCreateInfos( texture ) }
	{
	}

	void GenerateMipsCommand::apply( Context const & context )const
	{
		context.context->GenerateMips( m_view.getShaderView() );
	}

	CommandPtr GenerateMipsCommand::clone()const
	{
		return std::make_unique< GenerateMipsCommand >( *this );
	}
}
