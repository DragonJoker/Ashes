/*
This file belongs to RendererLib.
See LICENSE file in root folder.
*/
#include "D3D11CommandBase.hpp"

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
