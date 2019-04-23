#include "Buffer/D3D11Buffer.hpp"

#include "Core/D3D11Device.hpp"
#include "Miscellaneous/D3D11DeviceMemory.hpp"

namespace ashes::d3d11
{
	Buffer::Buffer( VkBufferCreateInfo createInfo )
		: m_createInfo{ std::move( createInfo ) }
	{
	}

	Buffer::~Buffer()
	{
	}

	VkMemoryRequirements Buffer::getMemoryRequirements()const
	{
		VkMemoryRequirements result{};
		result.alignment = 0u;
		result.memoryTypeBits = ~result.memoryTypeBits;
		result.size = m_createInfo.size;

		if ( checkFlag( m_createInfo.usage, VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT ) )
		{
			result.size = ashes::getAlignedSize( result.size, 16u );
		}

		return result;
	}

	void Buffer::doBindMemory()
	{
		m_buffer = static_cast< DeviceMemory & >( *m_storage ).bindToBuffer( m_target );

		if ( checkFlag( m_target, VK_BUFFER_USAGE_STORAGE_BUFFER_BIT )
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
			checkError( m_device, hr, "CreateUnorderedAccessViewBuffer" );
			dxDebugName( m_unorderedAccessView, UnorderedAccessViewBuffer );
		}
	}
}
