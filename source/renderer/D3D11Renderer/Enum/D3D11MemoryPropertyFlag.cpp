#include "D3D11RendererPrerequisites.hpp"

namespace ashes::d3d11
{
	bool isHostVisible( VkMemoryPropertyFlags const & flags )
	{
		return checkFlag( flags, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT )
			|| checkFlag( flags, VK_MEMORY_PROPERTY_HOST_CACHED_BIT )
			|| checkFlag( flags, VK_MEMORY_PROPERTY_HOST_COHERENT_BIT );
	}

	D3D11_USAGE getUsage( VkMemoryPropertyFlags const & flags
		, VkBufferUsageFlags const & targets )
	{
		D3D11_USAGE result{ D3D11_USAGE_DEFAULT };

		if ( isHostVisible( flags ) )
		{
			if ( isPipelineBindable( targets ) )
			{
				result = D3D11_USAGE_DYNAMIC;
			}
			else if ( checkFlag( targets, VK_BUFFER_USAGE_STORAGE_BUFFER_BIT )
				|| checkFlag( targets, VK_BUFFER_USAGE_STORAGE_TEXEL_BUFFER_BIT ) )
			{
			}
			else if ( checkFlag( targets, VK_BUFFER_USAGE_TRANSFER_SRC_BIT )
				|| checkFlag( targets, VK_BUFFER_USAGE_TRANSFER_DST_BIT ) )
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

	UINT getCpuAccessFlags( VkMemoryPropertyFlags const & flags
		, VkBufferUsageFlags const & targets )
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
				if ( checkFlag( targets, VK_BUFFER_USAGE_TRANSFER_SRC_BIT )
					|| checkFlag( targets, VK_BUFFER_USAGE_TRANSFER_DST_BIT ) )
				{
					result |= D3D11_CPU_ACCESS_WRITE;
					result |= D3D11_CPU_ACCESS_READ;
				}
			}
		}

		return result;
	}

	D3D11_USAGE getUsage( VkMemoryPropertyFlags const & flags
		, VkImageUsageFlags const & usage )
	{
		D3D11_USAGE result{ D3D11_USAGE_DEFAULT };

		if ( isHostVisible( flags ) )
		{
			if ( !isRenderTarget( usage ) )
			{
				if ( checkFlag( usage, VK_IMAGE_USAGE_TRANSFER_SRC_BIT )
					|| checkFlag( usage, VK_IMAGE_USAGE_TRANSFER_DST_BIT ) )
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

	UINT getCpuAccessFlags( VkMemoryPropertyFlags const & flags
		, VkImageUsageFlags const & usage )
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
				if ( checkFlag( usage, VK_IMAGE_USAGE_TRANSFER_SRC_BIT ) )
				{
					result |= D3D11_CPU_ACCESS_READ;
				}

				if ( checkFlag( usage, VK_IMAGE_USAGE_TRANSFER_DST_BIT ) )
				{
					result |= D3D11_CPU_ACCESS_WRITE;
				}
			}
		}

		return result;
	}
}
