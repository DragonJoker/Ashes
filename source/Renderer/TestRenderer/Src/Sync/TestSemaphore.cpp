/*
This file belongs to RendererLib.
See LICENSE file in root folder.
*/
#include "Sync/TestSemaphore.hpp"

#include "Core/TestDevice.hpp"

namespace test_renderer
{
	Semaphore::Semaphore( Device const & device )
		: renderer::Semaphore{ device }
	{
	}

	Semaphore::~Semaphore()
	{
	}
}
