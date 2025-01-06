#include "XBoxRendererPrerequisites.hpp"

namespace ashes::xbox
{
	D3D11_BLEND_OP getBlendOp( VkBlendOp const & value )
	{
		switch ( value )
		{
		case VK_BLEND_OP_ADD:
			return D3D11_BLEND_OP_ADD;

		case VK_BLEND_OP_SUBTRACT:
			return D3D11_BLEND_OP_SUBTRACT;

		case VK_BLEND_OP_REVERSE_SUBTRACT:
			return D3D11_BLEND_OP_REV_SUBTRACT;

		case VK_BLEND_OP_MIN:
			return D3D11_BLEND_OP_MIN;

		case VK_BLEND_OP_MAX:
			return D3D11_BLEND_OP_MAX;

		default:
			assert( false && "Unsupported blend operator" );
			return D3D11_BLEND_OP_ADD;
		}
	}
}
