/*
This file belongs to GlInstance.
See LICENSE file in root folder.
*/
#include "Command/Commands/GlViewportCommand.hpp"

#include "Core/GlDevice.hpp"
#include "RenderPass/GlFrameBuffer.hpp"

#include "ashesgl3_api.hpp"

using ashes::operator==;
using ashes::operator!=;

namespace ashes::gl3
{
	bool operator==( VkViewportArray const & lhs
		, VkViewportArray const & rhs )
	{
		auto result = lhs.size() == rhs.size();
		size_t index = 0u;

		while ( result && index < lhs.size() )
		{
			result = lhs[index] == rhs[index];
			++index;
		}

		return result;
	}

	namespace
	{
		VkViewport adjustViewport( ContextStateStack const & stack
			, VkViewport const & in )
		{
			VkViewport result{ in };

			if ( stack.hasCurrentFramebuffer() )
			{
				auto height = get( stack.getCurrentFramebuffer() )->getHeight();
				result.y = height - ( result.height + result.y );
			}

			return result;
		}

		VkViewportArray adjustViewports( ContextStateStack const & stack
			, VkViewportArray const & in )
		{
			VkViewportArray result;
			result.reserve( in.size() );

			for ( auto vp : in )
			{
				result.push_back( adjustViewport( stack, vp ) );
			}

			return result;
		}
	}

	void buildViewportCommand( ContextStateStack & stack
		, uint32_t firstViewport
		, VkViewportArray viewports
		, CmdList & list )
	{
		glLogCommand( "ViewportCommand" );
		viewports = adjustViewports( stack, viewports );
		stack.apply( list, viewports, false );
	}
}
