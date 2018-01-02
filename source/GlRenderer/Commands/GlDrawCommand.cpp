/*
This file belongs to GlRenderer.
See LICENSE file in root folder.
*/
#include "GlDrawCommand.hpp"

namespace gl_renderer
{
	DrawCommand::DrawCommand()
	{
	}

	void DrawCommand::apply()const
	{
	}

	CommandPtr DrawCommand::clone()const
	{
		return std::make_unique< DrawCommand >( *this );
	}
}
