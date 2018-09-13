#include "D3D11RendererPrerequisites.hpp"

namespace d3d11_renderer
{
	D3D11_MAP convert( renderer::MemoryMapFlags const & flags
		, renderer::MemoryPropertyFlags const & memory
		, renderer::BufferTargets const & targets )
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
				if ( checkFlag( flags, renderer::MemoryMapFlag::eRead ) )
				{
					result |= D3D11_MAP_READ;
				}

				if ( checkFlag( flags, renderer::MemoryMapFlag::eWrite ) )
				{
					result |= D3D11_MAP_WRITE;
				}
			}
		}
		else
		{
			if ( checkFlag( flags, renderer::MemoryMapFlag::eRead ) )
			{
				result |= D3D11_MAP_READ;
			}

			if ( checkFlag( flags, renderer::MemoryMapFlag::eWrite ) )
			{
				result |= D3D11_MAP_WRITE;
			}
		}

		return D3D11_MAP( result );
	}

	D3D11_MAP convert( renderer::MemoryMapFlags const & flags
		, renderer::MemoryPropertyFlags const & memory
		, renderer::ImageUsageFlags const & usage )
	{
		UINT result{ 0 };

		if ( isHostVisible( memory ) )
		{
			if ( !isRenderTarget( usage ) )
			{
				if ( checkFlag( flags, renderer::MemoryMapFlag::eRead ) )
				{
					result |= D3D11_MAP_READ;
				}

				if ( checkFlag( flags, renderer::MemoryMapFlag::eWrite ) )
				{
					result |= D3D11_MAP_WRITE;
				}
			}
		}
		else
		{
			if ( checkFlag( flags, renderer::MemoryMapFlag::eRead ) )
			{
				result |= D3D11_MAP_READ;
			}

			if ( checkFlag( flags, renderer::MemoryMapFlag::eWrite ) )
			{
				result |= D3D11_MAP_WRITE;
			}
		}

		return D3D11_MAP( result );
	}
}
