#include "Billboard.h"

#include "BillboardBuffer.h"

namespace render
{
	Billboard::Billboard( std::string const & name, BillboardBuffer & buffer )
		: Movable{ name }
		, m_buffer{ buffer }
	{
	}

	void Billboard::cull( renderer::StagingBuffer const & stagingBuffer
		, renderer::CommandBuffer const & commandBuffer
		, Camera const & camera
		, float scale )
	{
		m_buffer.cull( stagingBuffer
			, commandBuffer
			, camera
			, position()
			, scale );
	}
}
