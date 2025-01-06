#include "Buffer/XBoxBufferView.hpp"

#include "Buffer/XBoxBuffer.hpp"
#include "Core/XBoxDevice.hpp"

#include "ashesxbox_api.hpp"

namespace ashes::xbox
{
	BufferView::BufferView( VkDevice device
		, VkBufferViewCreateInfo createInfo )
		: m_device{ device }
		, m_createInfo{ std::move( createInfo ) }
	{
		D3D11_SHADER_RESOURCE_VIEW_DESC desc{};
		desc.Format = getSRVFormat( m_createInfo.format );
		desc.ViewDimension = D3D11_SRV_DIMENSION_BUFFER;
		auto elemSize = getSize( m_createInfo.format );
		desc.Buffer.FirstElement = UINT( m_createInfo.offset / elemSize );
		desc.Buffer.NumElements = UINT( m_createInfo.range / elemSize );

		auto hr = get( device )->getDevice()->CreateShaderResourceView( get( m_createInfo.buffer )->getResource()
			, &desc
			, &m_view );

		if ( checkError( device, hr, "CreateShaderResourceView" ) )
		{
			dxDebugName( m_view, ShaderResourceView );
		}
	}

	BufferView::~BufferView()noexcept
	{
		safeRelease( m_view );
	}
}
