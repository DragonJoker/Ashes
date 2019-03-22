#include "Buffer/D3D11Buffer.hpp"

#include "Core/D3D11Device.hpp"
#include "Miscellaneous/D3D11DeviceMemory.hpp"

#include <Ashes/Miscellaneous/MemoryRequirements.hpp>
#include <Ashes/Sync/BufferMemoryBarrier.hpp>

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
		ashes::MemoryRequirements result{};
		result.alignment = 0u;
		result.memoryTypeBits = ~result.memoryTypeBits;
		result.size = getSize();
		result.type = ashes::ResourceType::eBuffer;

		if ( checkFlag( m_target, ashes::BufferTarget::eUniformBuffer ) )
		{
			result.size = ashes::getAlignedSize( result.size, 16u );
		}

		return result;
	}

	void Buffer::doBindMemory()
	{
		m_buffer = static_cast< DeviceMemory & >( *m_storage ).bindToBuffer( m_target );

		if ( checkFlag( m_target, ashes::BufferTarget::eStorageBuffer )
			&& m_device.getFeatureLevel() >= D3D_FEATURE_LEVEL_11_0 )
		{
			auto device = m_device.getDevice();
			D3D11_UNORDERED_ACCESS_VIEW_DESC desc{};
			desc.Format = DXGI_FORMAT_R8_UINT;
			desc.ViewDimension = D3D11_UAV_DIMENSION_BUFFER;
			auto elemSize = 1u;
			desc.Buffer.FirstElement = 0u;
			desc.Buffer.NumElements = getSize();
			auto hr = device->CreateUnorderedAccessView( m_buffer
				, &desc
				, &m_unorderedAccessView );
			dxCheckError( hr, "CreateUnorderedAccessViewBuffer" );
			dxDebugName( m_unorderedAccessView, UnorderedAccessViewBuffer );
		}
	}
}
