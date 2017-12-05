/*
This file belongs to Renderer.
See LICENSE file in root folder.
*/
#include "StagingBuffer.hpp"

namespace renderer
{
	StagingBuffer::StagingBuffer( Device const & device
		, BufferTargets target
		, uint32_t size )
		: m_device{ device }
		, m_buffer{ device.createBuffer( size
			, target | BufferTarget::eTransferSrc
			, MemoryPropertyFlag::eHostVisible ) }
	{
	}
}
