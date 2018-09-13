#include "D3D11RendererPrerequisites.hpp"

namespace d3d11_renderer
{
	D3D11_MAP convert( ashes::MemoryMapFlags const & flags
		, ashes::MemoryPropertyFlags const & memory
		, ashes::BufferTargets const & targets )
	{
		UINT result{ 0 };

		if ( isHostVisible( memory ) )
		{
			if ( isPipelineBindable( targets ) )
			{
				result = D3D11_MAP_WRITE_DISCARD;
			}
			else
			{
				if ( checkFlag( flags, ashes::MemoryMapFlag::eRead ) )
				{
					result |= D3D11_MAP_READ;
				}

				if ( checkFlag( flags, ashes::MemoryMapFlag::eWrite ) )
				{
					result |= D3D11_MAP_WRITE;
				}
			}
		}
		else
		{
			if ( checkFlag( flags, ashes::MemoryMapFlag::eRead ) )
			{
				result |= D3D11_MAP_READ;
			}

			if ( checkFlag( flags, ashes::MemoryMapFlag::eWrite ) )
			{
				result |= D3D11_MAP_WRITE;
			}
		}

		return D3D11_MAP( result );
	}

	D3D11_MAP convert( ashes::MemoryMapFlags const & flags
		, ashes::MemoryPropertyFlags const & memory
		, ashes::ImageUsageFlags const & usage )
	{
		UINT result{ 0 };

		if ( isHostVisible( memory ) )
		{
			if ( !isRenderTarget( usage ) )
			{
				if ( checkFlag( flags, ashes::MemoryMapFlag::eRead ) )
				{
					result |= D3D11_MAP_READ;
				}

				if ( checkFlag( flags, ashes::MemoryMapFlag::eWrite ) )
				{
					result |= D3D11_MAP_WRITE;
				}
			}
		}
		else
		{
			if ( checkFlag( flags, ashes::MemoryMapFlag::eRead ) )
			{
				result |= D3D11_MAP_READ;
			}

			if ( checkFlag( flags, ashes::MemoryMapFlag::eWrite ) )
			{
				result |= D3D11_MAP_WRITE;
			}
		}

		return D3D11_MAP( result );
	}
}
