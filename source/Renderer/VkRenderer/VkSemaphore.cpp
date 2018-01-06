/*
This file belongs to Renderer.
See LICENSE file in root folder.
*/
#include "VkSemaphore.hpp"

#include "VkDevice.hpp"

namespace vk_renderer
{
	Semaphore::Semaphore( renderer::Device const & device )
		: renderer::Semaphore{ device }
		, m_semaphore{ static_cast< Device const & >( device ).getDevice() }
	{
	}
}
