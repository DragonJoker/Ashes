/*
This file belongs to GlInstance.
See LICENSE file in root folder.
*/
#include "Command/Commands/GlViewportCommand.hpp"

#include "Core/GlDevice.hpp"
#include "RenderPass/GlFramebuffer.hpp"

#include "ashesgl4_api.hpp"

using ashes::operator==;
using ashes::operator!=;

namespace ashes::gl4
{
	namespace
	{
		VkViewport adjustViewport( VkFramebuffer fbo
			, VkViewport const & in )
		{
			VkViewport result{ in };
			auto height = get( fbo )->getHeight();
			result.y = height - ( result.height + result.y );
			return result;
		}
	}

	void buildViewportCommand( ContextStateStack & stack
		, uint32_t firstViewport
		, VkViewportArray viewports
		, CmdList & list )
	{
		glLogCommand( "ViewportCommand" );
		auto viewport = adjustViewport( stack.getCurrentFramebuffer(), *viewports.begin() );

		if ( stack.getCurrentViewport() != viewport )
		{
			list.push_back( makeCmd< OpType::eApplyViewport >( viewport ) );
			stack.setCurrentViewport( viewport );
		}
	}
}
