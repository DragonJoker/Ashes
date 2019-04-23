/*
This file belongs to GlInstance.
See LICENSE file in root folder.
*/
#include "Command/Commands/GlViewportCommand.hpp"

#include "Core/GlDevice.hpp"

#include "ashesgl4_api.hpp"

namespace ashes::gl4
{
	void buildViewportCommand( uint32_t firstViewport
		, VkViewportArray viewports
		, CmdList & list )
	{
		glLogCommand( "ViewportCommand" );
		list.push_back( makeCmd< OpType::eApplyViewport >( *viewports.begin() ) );
	}
}
