/*
This file belongs to Ashes.
See LICENSE file in root folder.
*/
#include "Command/Commands/GlClearDepthStencilCommand.hpp"

#include "Core/GlContextStateStack.hpp"
#include "Image/GlImageView.hpp"
#include "Miscellaneous/GlCallLogger.hpp"

#include "ashesgl_api.hpp"

namespace ashes::gl
{
	void apply( ContextLock const & context
		, CmdClearTexDepth const & cmd )
	{
		glLogCall( context
			, glClearTexImage
			, cmd.name
			, cmd.mipLevel
			, cmd.format
			, cmd.type
			, &cmd.depth );
	}
	
	void apply( ContextLock const & context
		, CmdClearTexStencil const & cmd )
	{
		glLogCall( context
			, glClearTexImage
			, cmd.name
			, cmd.mipLevel
			, cmd.format
			, cmd.type
			, &cmd.stencil );
	}
	
	void apply( ContextLock const & context
		, CmdClearTexDepthStencil const & cmd )
	{
		glLogCall( context
			, glClearTexImage
			, cmd.name
			, cmd.mipLevel
			, cmd.format
			, cmd.type
			, &cmd.depthStencil.depth );
	}

	void buildClearDepthStencilCommand( VkDevice device
		, ContextStateStack & stack
		, VkImage image
		, VkImageLayout imageLayout
		, VkClearDepthStencilValue value
		, ArrayView< VkImageSubresourceRange const > ranges
		, CmdList & list )
	{
		glLogCommand( list, "ClearDepthStencilCommand" );
		stack.applyDisableBlend( list );

		if ( get( getInstance( device ) )->hasClearTexImage() )
		{
			auto format = get( image )->getUnpackFormat();
			auto type = get( image )->getUnpackType();

			if ( isDepthStencilFormat( get( image )->getFormatVk() ) )
			{
				for ( auto range : ranges )
				{
					if ( range.levelCount == RemainingArrayLayers )
					{
						range.levelCount = ashes::getMaxMipCount( get( image )->getDimensions() );
					}

					if ( range.layerCount == RemainingArrayLayers )
					{
						range.layerCount = get( device )->getLimits().maxImageArrayLayers;
					}

					for ( uint32_t level = range.baseMipLevel;
						level < range.baseMipLevel + range.levelCount;
						++level )
					{
						list.push_back( makeCmd< OpType::eClearTexDepthStencil >( get( image )->getInternal()
							, level
							, format
							, type
							, value ) );
					}
				}
			}
			else if ( isStencilFormat( get( image )->getFormatVk() ) )
			{
				for ( auto range : ranges )
				{
					if ( range.levelCount == RemainingArrayLayers )
					{
						range.levelCount = ashes::getMaxMipCount( get( image )->getDimensions() );
					}

					if ( range.layerCount == RemainingArrayLayers )
					{
						range.layerCount = get( device )->getLimits().maxImageArrayLayers;
					}

					for ( uint32_t level = range.baseMipLevel;
						level < range.baseMipLevel + range.levelCount;
						++level )
					{
						list.push_back( makeCmd< OpType::eClearTexStencil >( get( image )->getInternal()
							, level
							, format
							, type
							, int32_t( value.stencil ) ) );
					}
				}
			}
			else if ( isDepthFormat( get( image )->getFormatVk() ) )
			{
				for ( auto range : ranges )
				{
					if ( range.levelCount == RemainingArrayLayers )
					{
						range.levelCount = ashes::getMaxMipCount( get( image )->getDimensions() );
					}

					if ( range.layerCount == RemainingArrayLayers )
					{
						range.layerCount = get( device )->getLimits().maxImageArrayLayers;
					}

					for ( uint32_t level = range.baseMipLevel;
						level < range.baseMipLevel + range.levelCount;
						++level )
					{
						list.push_back( makeCmd< OpType::eClearTexDepth >( get( image )->getInternal()
							, level
							, format
							, type
							, value.depth ) );
					}
				}
			}
		}
		else
		{
			std::cerr << "Unsupported command : ClearDepthStencilCommand" << std::endl;
		}
	}
}
