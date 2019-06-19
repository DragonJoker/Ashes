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
	void buildScissorCommand( ContextStateStack & stack
		, uint32_t firstScissor
		, VkScissorArray scissors
		, CmdList & list )
	{
		glLogCommand( "ScissorCommand" );
		auto & scissor = *scissors.begin();

		if ( stack.getCurrentScissor() != scissor )
		{
			list.push_back( makeCmd< OpType::eApplyScissor >( scissor ) );
			stack.setCurrentScissor( scissor );
		}
	}
}
