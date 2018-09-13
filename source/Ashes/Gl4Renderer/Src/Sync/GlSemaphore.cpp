/*
This file belongs to Ashes.
See LICENSE file in root folder.
*/
#include "Sync/GlSemaphore.hpp"

#include "Core/GlDevice.hpp"

namespace gl_renderer
{
	Semaphore::Semaphore( ashes::Device const & device )
		: ashes::Semaphore{ device }
	{
	}
}
