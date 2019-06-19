/*
This file belongs to GlInstance.
See LICENSE file in root folder.
*/
#include "Command/Commands/GlViewportCommand.hpp"

#include "Core/GlDevice.hpp"

#include "ashesgl4_api.hpp"

using ashes::operator==;
using ashes::operator!=;

namespace ashes::gl4
{
	void buildViewportCommand( ContextStateStack & stack
		, uint32_t firstViewport
		, VkViewportArray viewports
		, CmdList & list )
	{
		glLogCommand( "ViewportCommand" );
		auto & viewport = *viewports.begin();

		if ( stack.getCurrentViewport() != viewport )
		{
			list.push_back( makeCmd< OpType::eApplyViewport >( viewport ) );
			stack.setCurrentViewport( viewport );
		}
	}
}
