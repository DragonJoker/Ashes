/*
This file belongs to GlInstance.
See LICENSE file in root folder.
*/
#include "Command/Commands/GlClearDepthStencilCommand.hpp"

#include "Image/GlImageView.hpp"
#include "Miscellaneous/GlCallLogger.hpp"

#include "ashesgl4_api.hpp"

namespace ashes::gl4
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
		, VkImage image
		, VkImageLayout imageLayout
		, VkClearDepthStencilValue value
		, VkImageSubresourceRangeArray ranges
		, CmdList & list )
	{
		glLogCommand( "ClearDepthStencilCommand" );

		if ( get( get( device )->getInstance() )->getContext().hasClearTexImage() )
		{
			auto internal = getInternalFormat( get( image )->getFormat() );
			auto format = getFormat( internal );
			auto type = getType( internal );

			if ( isDepthStencilFormat( get( image )->getFormat() ) )
			{
				for ( auto & range : ranges )
				{
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
			else if ( isStencilFormat( get( image )->getFormat() ) )
			{
				for ( auto & range : ranges )
				{
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
			else if ( isDepthFormat( get( image )->getFormat() ) )
			{
				for ( auto & range : ranges )
				{
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
