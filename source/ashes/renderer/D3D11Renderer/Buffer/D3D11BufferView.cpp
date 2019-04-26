#include "Buffer/D3D11BufferView.hpp"

#include "Buffer/D3D11Buffer.hpp"
#include "Core/D3D11Device.hpp"

#include "ashesd3d11_api.hpp"

namespace ashes::d3d11
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

		auto hr = get( device )->getDevice()->CreateShaderResourceView( get( m_createInfo.buffer )->getBuffer()
			, &desc
			, &m_view );

		if ( checkError( device, hr, "CreateShaderResourceView" ) )
		{
			dxDebugName( m_view, ShaderResourceView );
		}
	}

	BufferView::~BufferView()
	{
		safeRelease( m_view );
	}
}
