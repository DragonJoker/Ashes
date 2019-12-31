/*
This file belongs to Ashes.
See LICENSE file in root folder.
*/
#include "Command/Commands/GlClearColourCommand.hpp"

#include "Image/GlImage.hpp"
#include "Miscellaneous/GlCallLogger.hpp"

#include "ashesgl_api.hpp"

namespace ashes::gl
{
	void apply( ContextLock const & context
		, CmdClearTexColor const & cmd )
	{
		glLogCall( context
			, glClearTexImage
			, cmd.name
			, cmd.mipLevel
			, cmd.format
			, cmd.type
			, cmd.color.float32 );
	}

	void buildClearColourCommand( VkDevice device
		, VkImage image
		, VkImageLayout imageLayout
		, VkClearColorValue value
		, VkImageSubresourceRangeArray ranges
		, CmdList & list )
	{
		glLogCommand( "ClearColourCommand" );

		if ( get( get( device )->getInstance() )->hasClearTexImage() )
		{
			auto internal = getInternalFormat( get( image )->getFormat() );
			auto format = getFormat( internal );
			auto type = getType( internal );

			for ( auto & range : ranges )
			{
				for ( uint32_t level = range.baseMipLevel;
					level < range.baseMipLevel + range.levelCount;
					++level )
				{
					list.push_back( makeCmd< OpType::eClearTexColor >( get( image )->getInternal()
						, level
						, format
						, type
						, value ) );
				}
			}
		}
		else
		{
			std::cerr << "Unsupported command : ClearColourCommand" << std::endl;
		}
	}
}
