#include "VkRendererPrerequisites.hpp"

namespace vk_renderer
{
	VkClearValue convert( renderer::ClearValue const & value )
	{
		VkClearValue result;

		if ( value.isColour() )
		{
			result.color = convert( value.colour() );
		}
		else
		{
			result.depthStencil = convert( value.depthStencil() );
		}

		return result;
	}

	VkClearColorValue convert( renderer::ClearColorValue const & colour )
	{
		VkClearColorValue result;
		std::memcpy( &result, &colour, sizeof( colour ) );
		return result;
	}

	renderer::ClearColorValue  convert( VkClearColorValue const & colour )
	{
		return renderer::ClearColorValue
		{
			colour.float32[0],
			colour.float32[1],
			colour.float32[2],
			colour.float32[3],
		};
	}

	VkClearDepthStencilValue convert( renderer::DepthStencilClearValue const & value )
	{
		return VkClearDepthStencilValue{ value.depth, value.stencil };
	}

	renderer::DepthStencilClearValue convert( VkClearDepthStencilValue const & value )
	{
		return renderer::DepthStencilClearValue{ value.depth, value.stencil };
	}
}
