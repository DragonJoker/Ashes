#include "D3D11RendererPrerequisites.hpp"

namespace ashes::d3d11
{
	bool isPipelineBindable( VkBufferUsageFlags const & targets )
	{
		return checkFlag( targets, VK_BUFFER_USAGE_INDIRECT_BUFFER_BIT )
			|| checkFlag( targets, VK_BUFFER_USAGE_INDEX_BUFFER_BIT )
			|| checkFlag( targets, VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT )
			|| checkFlag( targets, VK_BUFFER_USAGE_UNIFORM_TEXEL_BUFFER_BIT )
			|| checkFlag( targets, VK_BUFFER_USAGE_VERTEX_BUFFER_BIT );
	}

	UINT getBindFlags( VkBufferUsageFlags const & targets )
	{
		UINT result{};

		if ( checkFlag( targets, VK_BUFFER_USAGE_INDEX_BUFFER_BIT ) )
		{
			result |= D3D11_BIND_INDEX_BUFFER;
		}

		if ( checkFlag( targets, VK_BUFFER_USAGE_INDIRECT_BUFFER_BIT ) )
		{
			result |= D3D11_BIND_SHADER_RESOURCE;
		}

		if ( checkFlag( targets, VK_BUFFER_USAGE_STORAGE_BUFFER_BIT ) )
		{
			result |= D3D11_BIND_UNORDERED_ACCESS;
		}

		if ( checkFlag( targets, VK_BUFFER_USAGE_STORAGE_TEXEL_BUFFER_BIT ) )
		{
			result |= D3D11_BIND_UNORDERED_ACCESS;
		}

		if ( checkFlag( targets, VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT ) )
		{
			result |= D3D11_BIND_CONSTANT_BUFFER;
		}

		if ( checkFlag( targets, VK_BUFFER_USAGE_UNIFORM_TEXEL_BUFFER_BIT ) )
		{
			result |= D3D11_BIND_SHADER_RESOURCE;
		}

		if ( checkFlag( targets, VK_BUFFER_USAGE_VERTEX_BUFFER_BIT ) )
		{
			result |= D3D11_BIND_VERTEX_BUFFER;
		}

		return result;
	}
}
