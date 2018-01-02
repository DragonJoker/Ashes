/*
This file belongs to Renderer.
See LICENSE file in root folder.
*/
#include "GlSemaphore.hpp"

#include "GlDevice.hpp"

namespace gl_renderer
{
	Semaphore::Semaphore( renderer::Device const & device )
		: renderer::Semaphore{ device }
	{
	}
}
