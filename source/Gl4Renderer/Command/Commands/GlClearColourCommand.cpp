/*
This file belongs to GlInstance.
See LICENSE file in root folder.
*/
#include "Command/Commands/GlClearColourCommand.hpp"

#include "Image/GlImage.hpp"
#include "Miscellaneous/GlCallLogger.hpp"

#include "ashesgl4_api.hpp"

namespace ashes::gl4
{
	ClearColourCommand::ClearColourCommand( VkDevice device
		, VkImage image
		, VkImageLayout imageLayout
		, VkClearColorValue value
		, VkImageSubresourceRangeArray ranges )
		: CommandBase{ device }
		, m_image{ image }
		, m_colour{ value }
		, m_internal{ getInternalFormat( get( m_image )->getFormat() ) }
		, m_format{ getFormat( m_internal ) }
		, m_type{ getType( m_internal ) }
	{
	}

	void ClearColourCommand::apply( ContextLock const & context )const
	{
		glLogCommand( "ClearColourCommand" );

		if ( context->hasClearTexImage() )
		{
			for ( auto & range : m_ranges )
			{
				for ( uint32_t level = range.baseMipLevel;
					level < range.baseMipLevel + range.levelCount;
					++level )
				{
					glLogCall( context
						, glClearTexImage
						, get( m_image )->getInternal()
						, level
						, m_format
						, m_type
						, m_colour.float32 );
				}
			}
		}
		else
		{
			std::cerr << "Unsupported command : ClearColourCommand" << std::endl;
		}
	}

	CommandPtr ClearColourCommand::clone()const
	{
		return std::make_unique< ClearColourCommand >( *this );
	}
}
