/*
This file belongs to Ashes.
See LICENSE file in root folder.
*/
#include "Sync/TestFence.hpp"

#include "Core/TestDevice.hpp"

namespace test_renderer
{
	Fence::Fence( Device const & device
		, ashes::FenceCreateFlags flags )
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
