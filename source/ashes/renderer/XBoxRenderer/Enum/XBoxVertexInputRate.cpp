#include "XBoxRendererPrerequisites.hpp"

namespace ashes::xbox
{
	D3D11_INPUT_CLASSIFICATION getInputClassification( VkVertexInputRate const & value )
	{
		switch ( value )
		{
		case VK_VERTEX_INPUT_RATE_VERTEX:
			return D3D11_INPUT_PER_VERTEX_DATA;

		case VK_VERTEX_INPUT_RATE_INSTANCE:
			return D3D11_INPUT_PER_INSTANCE_DATA;

		default:
			assert( false );
			return D3D11_INPUT_PER_VERTEX_DATA;
		}
	}
}
