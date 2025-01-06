#include "XBoxRendererPrerequisites.hpp"

namespace ashes::xbox
{
	D3D11_CULL_MODE getCullMode( VkCullModeFlags const & flags )
	{
		D3D11_CULL_MODE result( D3D11_CULL_NONE );

		if ( checkFlag( flags, VK_CULL_MODE_FRONT_BIT ) )
		{
			result = D3D11_CULL_FRONT;
		}
		else if ( checkFlag( flags, VK_CULL_MODE_BACK_BIT ) )
		{
			result = D3D11_CULL_BACK;
		}

		return result;
	}
}
