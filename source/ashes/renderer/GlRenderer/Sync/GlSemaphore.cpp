/*
This file belongs to Ashes.
See LICENSE file in root folder.
*/
#include "Sync/GlSemaphore.hpp"

#include "Core/GlDevice.hpp"

#include "ashesgl_api.hpp"

namespace ashes::gl
{
	Semaphore::Semaphore( [[maybe_unused]] VkAllocationCallbacks const * allocInfo
		, VkDevice device )
		: m_device{ device }
	{
		registerObject( m_device, *this );
	}

	Semaphore::~Semaphore()noexcept
	{
		unregisterObject( m_device, *this );
	}
}
