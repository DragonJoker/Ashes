#include "D3D11RendererPrerequisites.hpp"

namespace d3d11_renderer
{
	bool isPipelineBindable( renderer::BufferTargets const & targets )
	{
		return checkFlag( targets, renderer::BufferTarget::eDispatchIndirectBuffer )
			|| checkFlag( targets, renderer::BufferTarget::eDrawIndirectBuffer )
			|| checkFlag( targets, renderer::BufferTarget::eIndexBuffer )
			|| checkFlag( targets, renderer::BufferTarget::eStorageBuffer )
			|| checkFlag( targets, renderer::BufferTarget::eStorageTexelBuffer )
			|| checkFlag( targets, renderer::BufferTarget::eUniformBuffer )
			|| checkFlag( targets, renderer::BufferTarget::eUniformTexelBuffer )
			|| checkFlag( targets, renderer::BufferTarget::eVertexBuffer );
	}

	UINT convert( renderer::BufferTargets const & targets )
	{
		UINT result{};

		if ( checkFlag( targets, renderer::BufferTarget::eIndexBuffer ) )
		{
			result |= D3D11_BIND_INDEX_BUFFER;
		}

		if ( checkFlag( targets, renderer::BufferTarget::eDispatchIndirectBuffer )
			|| checkFlag( targets, renderer::BufferTarget::eDrawIndirectBuffer ) )
		{
			result |= D3D11_BIND_SHADER_RESOURCE;
		}

		if ( checkFlag( targets, renderer::BufferTarget::eStorageBuffer ) )
		{
			result |= D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_UNORDERED_ACCESS;
		}

		if ( checkFlag( targets, renderer::BufferTarget::eStorageTexelBuffer ) )
		{
			result |= D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_UNORDERED_ACCESS;
		}

		if ( checkFlag( targets, renderer::BufferTarget::eUniformBuffer ) )
		{
			result |= D3D11_BIND_CONSTANT_BUFFER;
		}

		if ( checkFlag( targets, renderer::BufferTarget::eUniformTexelBuffer ) )
		{
			result |= D3D11_BIND_SHADER_RESOURCE;
		}

		if ( checkFlag( targets, renderer::BufferTarget::eVertexBuffer ) )
		{
			result |= D3D11_BIND_VERTEX_BUFFER;
		}

		return result;
	}
}
