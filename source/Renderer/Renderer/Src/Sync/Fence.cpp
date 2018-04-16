/*
This file belongs to RendererLib.
See LICENSE file in root folder.
*/
#include "Sync/Fence.hpp"

#include "Core/Device.hpp"

namespace renderer
{
	Fence::Fence( Device const & device
		, FenceCreateFlags flags )
		: m_device{ device }
	{
		registerObject( m_device, "Fence", this );
	}

	Fence::~Fence()
	{
		unregisterObject( m_device, this );
	}
}
