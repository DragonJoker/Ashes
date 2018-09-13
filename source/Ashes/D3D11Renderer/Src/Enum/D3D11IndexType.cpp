#include "D3D11RendererPrerequisites.hpp"

namespace d3d11_renderer
{
	DXGI_FORMAT convert( ashes::IndexType const & type )
	{
		switch ( type )
		{
		case ashes::IndexType::eUInt16:
			return DXGI_FORMAT_R16_UINT;

		case ashes::IndexType::eUInt32:
			return DXGI_FORMAT_R32_UINT;

		default:
			assert( false && "Unsupported index type" );
			return DXGI_FORMAT_UNKNOWN;
		}
	}
}
