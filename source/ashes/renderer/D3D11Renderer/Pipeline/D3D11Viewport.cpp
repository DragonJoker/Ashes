/*
This file belongs to Ashes.
See LICENSE file in root folder.
*/
#include "D3D11RendererPrerequisites.hpp"

namespace ashes::d3d11
{
	D3D11_VIEWPORT makeViewport( VkViewport const & viewport )
	{
		return D3D11_VIEWPORT
		{
			viewport.x,
			viewport.y,
			viewport.width,
			viewport.height,
			viewport.minDepth,
			viewport.maxDepth
		};
	}

	std::vector< D3D11_VIEWPORT > makeViewports( ashes::VkViewportArray::const_iterator const & begin
		, ashes::VkViewportArray::const_iterator const & end )
	{
		std::vector< D3D11_VIEWPORT > result;

		for ( auto it = begin; it != end; ++it )
		{
			result.push_back( makeViewport( *it ) );
		}

		return result;
	}
}
