/*
This file belongs to GlInstance.
See LICENSE file in root folder.
*/
#include "Command/Commands/GlScissorCommand.hpp"

#include "Core/GlDevice.hpp"

#include "ashesgl4_api.hpp"

using ashes::operator==;
using ashes::operator!=;

namespace ashes::gl4
{
	namespace
	{
		VkRect2D adjustScissor( ContextStateStack const & stack
			, VkRect2D const & in )
		{
			VkRect2D result{ in };

			if ( stack.hasCurrentFramebuffer() )
			{
				auto height = get( stack.getCurrentFramebuffer() )->getHeight();
				result.offset.x = height - ( result.extent.height + result.offset.x );
			}

			return result;
		}

		VkScissorArray adjustScissors( ContextStateStack const & stack
			, VkScissorArray const & in )
		{
			VkScissorArray result;
			result.reserve( in.size() );

			for ( auto sc : in )
			{
				result.push_back( adjustScissor( stack, sc ) );
			}

			return result;
		}
	}

	void buildScissorCommand( ContextStateStack & stack
		, uint32_t firstScissor
		, VkScissorArray scissors
		, CmdList & list )
	{
		glLogCommand( "ScissorCommand" );
		scissors = adjustScissors( stack, scissors );
		stack.apply( list, scissors, false );
	}
}
