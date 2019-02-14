#include "D3D11RendererPrerequisites.hpp"

namespace d3d11_renderer
{
	D3D11_INPUT_CLASSIFICATION convert( ashes::VertexInputRate const & value )
	{
		switch ( value )
		{
		case ashes::VertexInputRate::eVertex:
			return D3D11_INPUT_PER_VERTEX_DATA;

		case ashes::VertexInputRate::eInstance:
			return D3D11_INPUT_PER_INSTANCE_DATA;

		default:
			assert( false );
			return D3D11_INPUT_PER_VERTEX_DATA;
		}
	}
}
