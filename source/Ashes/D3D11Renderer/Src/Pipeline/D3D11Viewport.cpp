/*
This file belongs to Ashes.
See LICENSE file in root folder.
*/
#include "D3D11RendererPrerequisites.hpp"

namespace d3d11_renderer
{
	D3D11_VIEWPORT makeViewport( ashes::Viewport const & viewport )
	{
		return D3D11_VIEWPORT
		{
			float( viewport.offset.x ),
			float( viewport.offset.y ),
			float( viewport.size.width ),
			float( viewport.size.height ),
			viewport.minDepth,
			viewport.maxDepth
		};
	}

	std::vector< D3D11_VIEWPORT > makeViewports( ashes::ViewportArray::const_iterator const & begin
		, ashes::ViewportArray::const_iterator const & end )
	{
		std::vector< D3D11_VIEWPORT > result;

		for ( auto it = begin; it != end; ++it )
		{
			result.push_back( makeViewport( *it ) );
		}

		return result;
	}
}
