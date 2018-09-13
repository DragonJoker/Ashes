#include "D3D11RendererPrerequisites.hpp"

namespace d3d11_renderer
{
	D3D11_BLEND_OP convert( ashes::BlendOp const & value )
	{
		switch ( value )
		{
		case ashes::BlendOp::eAdd:
			return D3D11_BLEND_OP_ADD;

		case ashes::BlendOp::eSubtract:
			return D3D11_BLEND_OP_SUBTRACT;

		case ashes::BlendOp::eReverseSubtract:
			return D3D11_BLEND_OP_REV_SUBTRACT;

		case ashes::BlendOp::eMin:
			return D3D11_BLEND_OP_MIN;

		case ashes::BlendOp::eMax:
			return D3D11_BLEND_OP_MAX;

		default:
			assert( false && "Unsupported blend operator" );
			return D3D11_BLEND_OP_ADD;
		}
	}
}
