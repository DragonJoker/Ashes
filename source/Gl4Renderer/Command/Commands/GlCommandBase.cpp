/*
This file belongs to Ashes.
See LICENSE file in root folder.
*/
#include "Command/Commands/GlCommandBase.hpp"

namespace ashes::gl4
{
	CommandBase::CommandBase( VkDevice device )
		: m_device{ device }
	{
	}

	CommandBase::~CommandBase()noexcept
	{
	}
}
