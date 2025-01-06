#include "XBoxRendererPrerequisites.hpp"

namespace ashes::xbox
{
	D3D11_MAP getBufferMapFlags( VkMemoryPropertyFlags const & memory
		, VkBufferUsageFlags const & usage )
	{
		UINT result{ 0 };

		if ( isHostVisible( memory ) )
		{
			if ( isPipelineBindable( usage ) )
			{
				result = D3D11_MAP_WRITE_DISCARD;
			}
			else
			{
				result |= D3D11_MAP_READ_WRITE;
			}
		}
		else
		{
			result |= D3D11_MAP_READ_WRITE;
		}

		return D3D11_MAP( result );
	}

	D3D11_MAP getImageMapFlags( VkMemoryPropertyFlags const & memory
		, VkImageUsageFlags const & usage )
	{
		UINT result{ 0 };

		if ( isHostVisible( memory ) )
		{
			if ( !isRenderTarget( usage ) )
			{
				result |= D3D11_MAP_READ_WRITE;
			}
		}
		else
		{
			result |= D3D11_MAP_READ_WRITE;
		}

		return D3D11_MAP( result );
	}
}
