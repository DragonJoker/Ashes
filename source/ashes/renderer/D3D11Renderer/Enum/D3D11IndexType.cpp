#include "D3D11RendererPrerequisites.hpp"

namespace ashes::D3D11_NAMESPACE
{
	DXGI_FORMAT getIndexFormat( VkIndexType const & type )
	{
		switch ( type )
		{
		case VK_INDEX_TYPE_UINT16:
			return DXGI_FORMAT_R16_UINT;

		case VK_INDEX_TYPE_UINT32:
			return DXGI_FORMAT_R32_UINT;

		default:
			assert( false && "Unsupported index type" );
			return DXGI_FORMAT_UNKNOWN;
		}
	}
}
