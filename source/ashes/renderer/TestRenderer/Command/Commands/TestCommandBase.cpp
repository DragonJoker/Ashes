/*
This file belongs to Ashes.
See LICENSE file in root folder.
*/
#include "Command/Commands/TestCommandBase.hpp"

#include "Core/TestDevice.hpp"

#include "ashestest_api.hpp"

namespace ashes::test
{
	CommandBase::CommandBase( VkDevice device )
		: m_device{ device }
	{
	}
}
