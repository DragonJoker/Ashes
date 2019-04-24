/*
This file belongs to Ashes.
See LICENSE file in root folder.
*/
#include "Sync/D3D11Fence.hpp"

#include "Core/D3D11Device.hpp"

namespace ashes::d3d11
{
	Fence::Fence( VkDevice device
		, VkFenceCreateFlags flags )
		: ashes::Fence{ device, flags }
	{
	}

	Fence::~Fence()
	{
	}

	ashes::WaitResult Fence::wait( uint64_t timeout )const
	{
		return ashes::WaitResult::eSuccess;
	}

	void Fence::reset()const
	{
	}
}
