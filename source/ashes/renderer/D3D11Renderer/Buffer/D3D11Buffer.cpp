#include "Buffer/D3D11Buffer.hpp"

#include "Core/D3D11Device.hpp"
#include "Miscellaneous/D3D11DeviceMemory.hpp"

#include "ashesd3d11_api.hpp"

namespace ashes::d3d11
{
	Buffer::Buffer( VkDevice device
		, VkBufferCreateInfo createInfo )
		: m_device{ device }
		, m_createInfo{ std::move( createInfo ) }
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
		m_buffer = get( m_memory )->bindToBuffer( m_createInfo.usage );

		if ( checkFlag( m_createInfo.usage, VK_BUFFER_USAGE_STORAGE_BUFFER_BIT )
			&& get( m_device )->getFeatureLevel() >= D3D_FEATURE_LEVEL_11_0 )
		{
			auto device = get( m_device )->getDevice();
			D3D11_UNORDERED_ACCESS_VIEW_DESC desc{};
			desc.Format = DXGI_FORMAT_R8_UINT;
			desc.ViewDimension = D3D11_UAV_DIMENSION_BUFFER;
			auto elemSize = 1u;
			desc.Buffer.FirstElement = 0u;
			desc.Buffer.NumElements = m_createInfo.size;
			auto hr = device->CreateUnorderedAccessView( m_buffer
				, &desc
				, &m_unorderedAccessView );
			checkError( *get( m_device ), hr, "CreateUnorderedAccessViewBuffer" );
			dxDebugName( m_unorderedAccessView, UnorderedAccessViewBuffer );
		}
	}
}
