/*
This file belongs to Ashes.
See LICENSE file in root folder.
*/
#include "Command/Commands/D3D11CommandBase.hpp"

namespace ashes::d3d11
{
	CommandBase::CommandBase( VkDevice device )
		: m_device{ device }
	{
	}

	CommandBase::~CommandBase()noexcept
	{
	}
}
