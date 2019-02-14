/*
This file belongs to Ashes.
See LICENSE file in root folder.
*/
#include "Command/Commands/GlCommandBase.hpp"

namespace gl_renderer
{
	CommandBase::CommandBase( Device const & device )
		: m_device{ device }
	{
	}

	CommandBase::~CommandBase()noexcept
	{
	}
}
