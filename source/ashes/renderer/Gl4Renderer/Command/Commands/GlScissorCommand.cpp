/*
This file belongs to GlInstance.
See LICENSE file in root folder.
*/
#include "Command/Commands/GlScissorCommand.hpp"

#include "Core/GlDevice.hpp"

#include "ashesgl4_api.hpp"

namespace ashes::gl4
{
	void buildScissorCommand( uint32_t firstScissor
		, VkScissorArray scissors
		, CmdList & list )
	{
		glLogCommand( "ScissorCommand" );
		list.push_back( makeCmd< OpType::eApplyScissor >( *scissors.begin() ) );
	}
}
