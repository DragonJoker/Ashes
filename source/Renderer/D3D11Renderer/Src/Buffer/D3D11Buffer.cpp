#include "Buffer/D3D11Buffer.hpp"

#include "Core/D3D11Device.hpp"
#include "Miscellaneous/D3D11DeviceMemory.hpp"

#include <Miscellaneous/MemoryRequirements.hpp>
#include <Sync/BufferMemoryBarrier.hpp>

namespace d3d11_renderer
{
	Buffer::Buffer( Device const & device
		, uint32_t size
		, renderer::BufferTargets target )
		: renderer::BufferBase{ device
			, size
			, target }
		, m_device{ device }
	{
	}

	Buffer::~Buffer()
	{
	}

	renderer::MemoryRequirements Buffer::getMemoryRequirements()const
	{
		renderer::MemoryRequirements result;
		result.alignment = 0u;
		result.memoryTypeBits = 0xFFFFFFFF;
		result.size = getSize();
		result.type = renderer::ResourceType::eBuffer;
		return result;
	}

	void Buffer::doBindMemory()
	{
		m_buffer = static_cast< DeviceMemory & >( *m_storage ).bindToBuffer( m_target );
	}
}
