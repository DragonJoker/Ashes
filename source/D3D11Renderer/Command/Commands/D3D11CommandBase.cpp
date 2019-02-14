/*
This file belongs to Ashes.
See LICENSE file in root folder.
*/
#include "Command/Commands/D3D11CommandBase.hpp"

namespace d3d11_renderer
{
	CommandBase::CommandBase( Device const & device )
		: m_device{ device }
	{
	}

	CommandBase::~CommandBase()noexcept
	{
	}
}
