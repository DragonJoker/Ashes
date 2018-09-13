#include "D3D11RendererPrerequisites.hpp"

namespace d3d11_renderer
{
	D3D11_CULL_MODE convert( ashes::CullModeFlags const & flags )
	{
		D3D11_CULL_MODE result( D3D11_CULL_NONE );

		if ( checkFlag( flags, ashes::CullModeFlag::eFront ) )
		{
			result = D3D11_CULL_FRONT;
		}
		else if ( checkFlag( flags, ashes::CullModeFlag::eBack ) )
		{
			result = D3D11_CULL_BACK;
		}

		return result;
	}
}
