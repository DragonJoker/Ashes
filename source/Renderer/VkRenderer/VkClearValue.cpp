#include "VkClearValue.hpp"

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

	VkClearColorValue convert( utils::RgbaColour const & colour )
	{
		return VkClearColorValue
		{
			{ colour.r, colour.g, colour.b, colour.a }
		};
	}

	utils::RgbaColour convert( VkClearColorValue const & colour )
	{
		return utils::RgbaColour
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
