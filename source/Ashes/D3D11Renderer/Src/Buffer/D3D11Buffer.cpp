#include "Buffer/D3D11Buffer.hpp"

#include "Core/D3D11Device.hpp"
#include "Miscellaneous/D3D11DeviceMemory.hpp"

#include <Miscellaneous/MemoryRequirements.hpp>
#include <Sync/BufferMemoryBarrier.hpp>

namespace d3d11_renderer
{
	Buffer::Buffer( Device const & device
		, uint32_t size
		, ashes::BufferTargets target )
		: ashes::BufferBase{ device
			, size
			, target }
		, m_device{ device }
	{
	}

	Buffer::~Buffer()
	{
	}

	ashes::MemoryRequirements Buffer::getMemoryRequirements()const
	{
		ashes::MemoryRequirements result;
		result.alignment = 0u;
		result.memoryTypeBits = 0xFFFFFFFF;
		result.size = getSize();
		result.type = ashes::ResourceType::eBuffer;
		return result;
	}

	void Buffer::doBindMemory()
	{
		m_buffer = static_cast< DeviceMemory & >( *m_storage ).bindToBuffer( m_target );
	}
}
