#include "XBoxRendererPrerequisites.hpp"

namespace ashes::xbox
{
	D3D11_FILTER_TYPE getFilterType( VkFilter const & filter )
	{
		switch ( filter )
		{
		case VK_FILTER_NEAREST:
			return D3D11_FILTER_TYPE_POINT;

		case VK_FILTER_LINEAR:
			return D3D11_FILTER_TYPE_LINEAR;

		default:
			assert( false );
			return D3D11_FILTER_TYPE_POINT;
		}
	}

}
