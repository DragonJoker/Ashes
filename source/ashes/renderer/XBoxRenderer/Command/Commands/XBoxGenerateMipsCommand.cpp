/*
This file belongs to Ashes.
See LICENSE file in root folder.
*/
#include "Command/Commands/XBoxGenerateMipsCommand.hpp"

#include "Image/XBoxImage.hpp"

#include "ashesxbox_api.hpp"

namespace ashes::xbox
{
	namespace
	{
		VkImageViewCreateInfo getCreateInfos( VkImage texture )
		{
			VkImageViewCreateInfo result{};
			result.image = texture;
			result.viewType = VkImageViewType( get( texture )->getType() );
			result.format = get( texture )->getFormat();
			result.subresourceRange.aspectMask = getAspectMask( get( texture )->getFormat() );
			result.subresourceRange.baseArrayLayer = 0u;
			result.subresourceRange.baseMipLevel = 0u;
			result.subresourceRange.layerCount = get( texture )->getLayerCount();
			result.subresourceRange.levelCount = get( texture )->getMipmapLevels();
			return result;
		}
	}

	GenerateMipsCommand::GenerateMipsCommand( VkDevice device
		, VkImage texture )
		: CommandBase{ device }
		, m_texture{ texture }
		, m_view{ device, getCreateInfos( texture ) }
	{
	}

	void GenerateMipsCommand::apply( Context const & context )const
	{
		context.context->GenerateMips( m_view.getShaderView() );
	}

	CommandPtr GenerateMipsCommand::clone()const
	{
		return std::make_unique< GenerateMipsCommand >( getDevice(), m_texture );
	}
}
