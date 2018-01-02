/*
This file belongs to GlRenderer.
See LICENSE file in root folder.
*/
#include "GlCopyVertexBufferCommand.hpp"

namespace gl_renderer
{
	CopyVertexBufferCommand::CopyVertexBufferCommand()
	{
	}

	void CopyVertexBufferCommand::apply()const
	{
	}

	CommandPtr CopyVertexBufferCommand::clone()const
	{
		return std::make_unique< CopyVertexBufferCommand >( *this );
	}
}
