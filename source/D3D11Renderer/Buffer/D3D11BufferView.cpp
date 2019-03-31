#include "Buffer/D3D11BufferView.hpp"

#include "Buffer/D3D11Buffer.hpp"
#include "Core/D3D11Device.hpp"

namespace d3d11_renderer
{
	BufferView::BufferView( Device const & device
		, Buffer const & buffer
		, ashes::Format format
		, uint32_t offset
		, uint32_t range )
		: ashes::BufferView{ device, buffer, format, offset, range }
	{
		D3D11_SHADER_RESOURCE_VIEW_DESC desc{};
		desc.Format = getSRVFormat( format );
		desc.ViewDimension = D3D11_SRV_DIMENSION_BUFFER;
		auto elemSize = getSize( format );
		desc.Buffer.FirstElement = offset / elemSize;
		desc.Buffer.NumElements = range / elemSize;

		auto hr = device.getDevice()->CreateShaderResourceView( buffer.getBuffer()
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
