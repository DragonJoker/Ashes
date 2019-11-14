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
		safeRelease( m_unorderedAccessView );
	}

	VkMemoryRequirements Buffer::getMemoryRequirements()const
	{
		VkMemoryRequirements result{};
		result.memoryTypeBits = ~( 0u );
		result.alignment = get( m_device )->getLimits().nonCoherentAtomSize;

		if ( checkFlag( m_createInfo.usage, VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT ) )
		{
			result.alignment = get( m_device )->getLimits().minUniformBufferOffsetAlignment;
		}

		result.size = ashes::getAlignedSize( m_createInfo.size, result.alignment );
		return result;
	}

	bool Buffer::isMapped()const
	{
		assert( m_memory != VK_NULL_HANDLE );
		return get( m_memory )->isMapped();
	}

	void Buffer::copyFrom( ID3D11DeviceContext * context
		, VkBuffer src
		, D3D11_BOX const & srcBox
		, UINT dstOffset )const
	{
		VkDeviceSize size = srcBox.right - srcBox.left;
		get( m_memory )->updateData( get( src )->m_memory
			, VkDeviceSize( srcBox.left )
			, VkDeviceSize( dstOffset )
			, size );
		context->CopySubresourceRegion( getResource()
			, 0u
			, dstOffset
			, 0u
			, 0u
			, get( src )->getResource()
			, 0u
			, &srcBox );
	}

	VkResult Buffer::bindMemory( VkDeviceMemory memory
		, VkDeviceSize memoryOffset )
	{
		m_memory = memory;
		m_memoryOffset = memoryOffset;
		auto result = get( m_memory )->bindToBuffer( get( this )
			, m_memoryOffset
			, m_objectMemory );

		if ( !m_debugName.empty() )
		{
			m_objectMemory->resource->SetPrivateData( WKPDID_D3DDebugObjectName
				, UINT( m_debugName.size() )
				, m_debugName.c_str() );
		}

		if ( checkFlag( m_createInfo.usage, VK_BUFFER_USAGE_STORAGE_BUFFER_BIT )
			&& get( m_device )->getFeatureLevel() >= D3D_FEATURE_LEVEL_11_0 )
		{
			auto device = get( m_device )->getDevice();
			D3D11_UNORDERED_ACCESS_VIEW_DESC desc{};
			desc.Format = DXGI_FORMAT_R8_UINT;
			desc.ViewDimension = D3D11_UAV_DIMENSION_BUFFER;
			auto elemSize = 1u;
			desc.Buffer.FirstElement = 0u;
			desc.Buffer.NumElements = UINT( m_createInfo.size );
			auto hr = device->CreateUnorderedAccessView( m_objectMemory->resource
				, &desc
				, &m_unorderedAccessView );
			checkError( m_device, hr, "CreateUnorderedAccessViewBuffer" );
			dxDebugName( m_unorderedAccessView, UnorderedAccessViewBuffer );
		}

		return result;
	}
}
