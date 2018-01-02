/*
This file belongs to GlRenderer.
See LICENSE file in root folder.
*/
#include "GlBindIndexBufferCommand.hpp"

namespace gl_renderer
{
	BindIndexBufferCommand::BindIndexBufferCommand()
	{
	}

	void BindIndexBufferCommand::apply()const
	{
	}

	CommandPtr BindIndexBufferCommand::clone()const
	{
		return std::make_unique< BindIndexBufferCommand >( *this );
	}
}
