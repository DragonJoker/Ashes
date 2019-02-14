#include "D3D11RendererPrerequisites.hpp"

namespace d3d11_renderer
{
	bool isPipelineBindable( ashes::BufferTargets const & targets )
	{
		return checkFlag( targets, ashes::BufferTarget::eDispatchIndirectBuffer )
			|| checkFlag( targets, ashes::BufferTarget::eDrawIndirectBuffer )
			|| checkFlag( targets, ashes::BufferTarget::eIndexBuffer )
			|| checkFlag( targets, ashes::BufferTarget::eUniformBuffer )
			|| checkFlag( targets, ashes::BufferTarget::eUniformTexelBuffer )
			|| checkFlag( targets, ashes::BufferTarget::eVertexBuffer );
	}

	UINT convert( ashes::BufferTargets const & targets )
	{
		UINT result{};

		if ( checkFlag( targets, ashes::BufferTarget::eIndexBuffer ) )
		{
			result |= D3D11_BIND_INDEX_BUFFER;
		}

		if ( checkFlag( targets, ashes::BufferTarget::eDispatchIndirectBuffer )
			|| checkFlag( targets, ashes::BufferTarget::eDrawIndirectBuffer ) )
		{
			result |= D3D11_BIND_SHADER_RESOURCE;
		}

		if ( checkFlag( targets, ashes::BufferTarget::eStorageBuffer ) )
		{
			result |= D3D11_BIND_UNORDERED_ACCESS;
		}

		if ( checkFlag( targets, ashes::BufferTarget::eStorageTexelBuffer ) )
		{
			result |= D3D11_BIND_UNORDERED_ACCESS;
		}

		if ( checkFlag( targets, ashes::BufferTarget::eUniformBuffer ) )
		{
			result |= D3D11_BIND_CONSTANT_BUFFER;
		}

		if ( checkFlag( targets, ashes::BufferTarget::eUniformTexelBuffer ) )
		{
			result |= D3D11_BIND_SHADER_RESOURCE;
		}

		if ( checkFlag( targets, ashes::BufferTarget::eVertexBuffer ) )
		{
			result |= D3D11_BIND_VERTEX_BUFFER;
		}

		return result;
	}
}
