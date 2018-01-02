/*
This file belongs to GlRenderer.
See LICENSE file in root folder.
*/
#include "GlDrawIndexedCommand.hpp"

namespace gl_renderer
{
	DrawIndexedCommand::DrawIndexedCommand()
	{
	}

	void DrawIndexedCommand::apply()const
	{
	}

	CommandPtr DrawIndexedCommand::clone()const
	{
		return std::make_unique< DrawIndexedCommand >( *this );
	}
}
