#include "D3D11RendererPrerequisites.hpp"

namespace d3d11_renderer
{
	D3D11_INPUT_CLASSIFICATION convert( renderer::VertexInputRate const & value )
	{
		switch ( value )
		{
		case renderer::VertexInputRate::eVertex:
			return D3D11_INPUT_PER_VERTEX_DATA;

		case renderer::VertexInputRate::eInstance:
			return D3D11_INPUT_PER_INSTANCE_DATA;

		default:
			assert( false );
			return D3D11_INPUT_PER_VERTEX_DATA;
		}
	}
}
