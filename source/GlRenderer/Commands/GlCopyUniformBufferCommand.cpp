/*
This file belongs to GlRenderer.
See LICENSE file in root folder.
*/
#include "GlCopyUniformBufferCommand.hpp"

namespace gl_renderer
{
	CopyUniformBufferCommand::CopyUniformBufferCommand()
	{
	}

	void CopyUniformBufferCommand::apply()const
	{
	}

	CommandPtr CopyUniformBufferCommand::clone()const
	{
		return std::make_unique< CopyUniformBufferCommand >( *this );
	}
}
