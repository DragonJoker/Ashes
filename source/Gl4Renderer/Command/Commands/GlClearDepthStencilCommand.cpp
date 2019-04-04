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
	ClearDepthStencilCommand::ClearDepthStencilCommand( VkDevice device
		, VkImage image
		, VkImageLayout imageLayout
		, VkClearDepthStencilValue value
		, VkImageSubresourceRangeArray ranges )
		: CommandBase{ device }
		, m_image{ image }
		, m_value{ value }
		, m_ranges{ ranges }
		, m_internal{ getInternalFormat( get( m_image )->getFormat() ) }
		, m_format{ getFormat( m_internal ) }
		, m_type{ getType( m_internal ) }
	{
	}

	void ClearDepthStencilCommand::apply( ContextLock const & context )const
	{
		glLogCommand( "ClearDepthStencilCommand" );

		if ( context->hasClearTexImage() )
		{
			if ( isDepthStencilFormat( get( m_image )->getFormat() ) )
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
							, &m_value.depth );
					}
				}
			}
			else if ( isStencilFormat( get( m_image )->getFormat() ) )
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
							, &m_value.stencil );
					}
				}
			}
			else if ( isDepthFormat( get( m_image )->getFormat() ) )
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
							, &m_value.depth );
					}
				}
			}
		}
		else
		{
			std::cerr << "Unsupported command : ClearDepthStencilCommand" << std::endl;
		}
	}

	CommandPtr ClearDepthStencilCommand::clone()const
	{
		return std::make_unique< ClearDepthStencilCommand >( *this );
	}
}
