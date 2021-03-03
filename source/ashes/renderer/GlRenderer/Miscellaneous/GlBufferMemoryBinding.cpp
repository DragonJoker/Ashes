/*
This file belongs to Ashes.
See LICENSE file in root folder
*/
#include "Miscellaneous/GlBufferMemoryBinding.hpp"

#include "Buffer/GlBuffer.hpp"

#include "ashesgl_api.hpp"

namespace ashes::gl
{
	BufferMemoryBinding::BufferMemoryBinding( VkDeviceMemory parent
		, VkDevice device
		, VkBuffer buffer
		, VkDeviceSize memoryOffset )
		: DeviceMemoryBinding{ parent
			, device
			, get( buffer )->getTarget()
			, memoryOffset
			, get( buffer )->getMemoryRequirements()
			, buffer
			, get( parent )->getInternal() }
		, m_buffer{ get( buffer ) }
	{
		m_buffer->setMemoryBinding( this );
	}

	BufferMemoryBinding::~BufferMemoryBinding()
	{
		m_buffer->setMemoryBinding( nullptr );
	}
}
