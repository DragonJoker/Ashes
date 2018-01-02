/*
This file belongs to GlRenderer.
See LICENSE file in root folder.
*/
#include "GlCopyImageCommand.hpp"

namespace gl_renderer
{
	CopyImageCommand::CopyImageCommand()
	{
	}

	void CopyImageCommand::apply()const
	{
	}

	CommandPtr CopyImageCommand::clone()const
	{
		return std::make_unique< CopyImageCommand >( *this );
	}
}
