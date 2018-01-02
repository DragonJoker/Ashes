/*
This file belongs to GlRenderer.
See LICENSE file in root folder.
*/
#include "GlBindVertexBufferCommand.hpp"

namespace gl_renderer
{
	BindVertexBufferCommand::BindVertexBufferCommand()
	{
	}

	void BindVertexBufferCommand::apply()const
	{
	}

	CommandPtr BindVertexBufferCommand::clone()const
	{
		return std::make_unique< BindVertexBufferCommand >( *this );
	}
}
