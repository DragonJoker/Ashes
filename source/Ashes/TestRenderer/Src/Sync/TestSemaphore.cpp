/*
This file belongs to Ashes.
See LICENSE file in root folder.
*/
#include "Sync/TestSemaphore.hpp"

#include "Core/TestDevice.hpp"

namespace test_renderer
{
	Semaphore::Semaphore( Device const & device )
		: ashes::Semaphore{ device }
	{
	}

	Semaphore::~Semaphore()
	{
	}
}
