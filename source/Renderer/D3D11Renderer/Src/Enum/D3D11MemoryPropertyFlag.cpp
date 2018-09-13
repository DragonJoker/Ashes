#include "D3D11RendererPrerequisites.hpp"

namespace d3d11_renderer
{
	bool isHostVisible( renderer::MemoryPropertyFlags const & flags )
	{
		return checkFlag( flags, renderer::MemoryPropertyFlag::eHostVisible )
			|| checkFlag( flags, renderer::MemoryPropertyFlag::eHostCached )
			|| checkFlag( flags, renderer::MemoryPropertyFlag::eHostCoherent );
	}

	D3D11_USAGE getUsage( renderer::MemoryPropertyFlags const & flags
		, renderer::BufferTargets const & targets )
	{
		D3D11_USAGE result{ D3D11_USAGE_DEFAULT };

		if ( isHostVisible( flags ) )
		{
			if ( isPipelineBindable( targets ) )
			{
				result = D3D11_USAGE_DYNAMIC;
			}
			else
			{
				if ( checkFlag( targets, renderer::BufferTarget::eTransferSrc )
					|| checkFlag( targets, renderer::BufferTarget::eTransferDst ) )
				{
					result = D3D11_USAGE_STAGING;
				}
				else
				{
					result = D3D11_USAGE_DYNAMIC;
				}
			}
		}

		return result;
	}

	UINT getCpuAccessFlags( renderer::MemoryPropertyFlags const & flags
		, renderer::BufferTargets const & targets )
	{
		UINT result{};

		if ( isHostVisible( flags ) )
		{
			if ( isPipelineBindable( targets ) )
			{
				result = D3D11_CPU_ACCESS_WRITE;
			}
			else
			{
				if ( checkFlag( targets, renderer::BufferTarget::eTransferSrc ) )
				{
					result |= D3D11_CPU_ACCESS_READ;
				}

				if ( checkFlag( targets, renderer::BufferTarget::eTransferDst ) )
				{
					result |= D3D11_CPU_ACCESS_WRITE;
				}
			}
		}

		return result;
	}

	D3D11_USAGE getUsage( renderer::MemoryPropertyFlags const & flags
		, renderer::ImageUsageFlags const & usage )
	{
		D3D11_USAGE result{ D3D11_USAGE_DEFAULT };

		if ( isHostVisible( flags ) )
		{
			if ( !isRenderTarget( usage ) )
			{
				if ( checkFlag( usage, renderer::ImageUsageFlag::eTransferSrc )
					|| checkFlag( usage, renderer::ImageUsageFlag::eTransferDst ) )
				{
					result = D3D11_USAGE_STAGING;
				}
				else
				{
					result = D3D11_USAGE_DYNAMIC;
				}
			}
		}

		return result;
	}

	UINT getCpuAccessFlags( renderer::MemoryPropertyFlags const & flags
		, renderer::ImageUsageFlags const & usage )
	{
		UINT result{};

		if ( isHostVisible( flags ) )
		{
			if ( !isRenderTarget( usage ) )
			{
				result = D3D11_CPU_ACCESS_WRITE;
			}
			else
			{
				if ( checkFlag( usage, renderer::ImageUsageFlag::eTransferSrc ) )
				{
					result |= D3D11_CPU_ACCESS_READ;
				}

				if ( checkFlag( usage, renderer::ImageUsageFlag::eTransferDst ) )
				{
					result |= D3D11_CPU_ACCESS_WRITE;
				}
			}
		}

		return result;
	}
}
