#include "D3D11RendererPrerequisites.hpp"

namespace d3d11_renderer
{
	bool isHostVisible( ashes::MemoryPropertyFlags const & flags )
	{
		return checkFlag( flags, ashes::MemoryPropertyFlag::eHostVisible )
			|| checkFlag( flags, ashes::MemoryPropertyFlag::eHostCached )
			|| checkFlag( flags, ashes::MemoryPropertyFlag::eHostCoherent );
	}

	D3D11_USAGE getUsage( ashes::MemoryPropertyFlags const & flags
		, ashes::BufferTargets const & targets )
	{
		D3D11_USAGE result{ D3D11_USAGE_DEFAULT };

		if ( isHostVisible( flags ) )
		{
			if ( isPipelineBindable( targets ) )
			{
				result = D3D11_USAGE_DYNAMIC;
			}
			else if ( checkFlag( targets, ashes::BufferTarget::eStorageBuffer )
				|| checkFlag( targets, ashes::BufferTarget::eStorageTexelBuffer ) )
			{
			}
			else if ( checkFlag( targets, ashes::BufferTarget::eTransferSrc )
				|| checkFlag( targets, ashes::BufferTarget::eTransferDst ) )
			{
				result = D3D11_USAGE_STAGING;
			}
			else
			{
				result = D3D11_USAGE_DYNAMIC;
			}
		}

		return result;
	}

	UINT getCpuAccessFlags( ashes::MemoryPropertyFlags const & flags
		, ashes::BufferTargets const & targets )
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
				if ( checkFlag( targets, ashes::BufferTarget::eTransferSrc )
					|| checkFlag( targets, ashes::BufferTarget::eTransferDst ) )
				{
					result |= D3D11_CPU_ACCESS_WRITE;
					result |= D3D11_CPU_ACCESS_READ;
				}
			}
		}

		return result;
	}

	D3D11_USAGE getUsage( ashes::MemoryPropertyFlags const & flags
		, ashes::ImageUsageFlags const & usage )
	{
		D3D11_USAGE result{ D3D11_USAGE_DEFAULT };

		if ( isHostVisible( flags ) )
		{
			if ( !isRenderTarget( usage ) )
			{
				if ( checkFlag( usage, ashes::ImageUsageFlag::eTransferSrc )
					|| checkFlag( usage, ashes::ImageUsageFlag::eTransferDst ) )
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

	UINT getCpuAccessFlags( ashes::MemoryPropertyFlags const & flags
		, ashes::ImageUsageFlags const & usage )
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
				if ( checkFlag( usage, ashes::ImageUsageFlag::eTransferSrc ) )
				{
					result |= D3D11_CPU_ACCESS_READ;
				}

				if ( checkFlag( usage, ashes::ImageUsageFlag::eTransferDst ) )
				{
					result |= D3D11_CPU_ACCESS_WRITE;
				}
			}
		}

		return result;
	}
}
