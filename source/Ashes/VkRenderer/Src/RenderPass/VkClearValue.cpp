#include "VkRendererPrerequisites.hpp"

namespace vk_renderer
{
	VkClearValue convert( ashes::ClearValue const & value )
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

	VkClearColorValue convert( ashes::ClearColorValue const & colour )
	{
		VkClearColorValue result;
		std::memcpy( &result, &colour, sizeof( colour ) );
		return result;
	}

	ashes::ClearColorValue  convert( VkClearColorValue const & colour )
	{
		return ashes::ClearColorValue
		{
			colour.float32[0],
			colour.float32[1],
			colour.float32[2],
			colour.float32[3],
		};
	}

	VkClearDepthStencilValue convert( ashes::DepthStencilClearValue const & value )
	{
		return VkClearDepthStencilValue{ value.depth, value.stencil };
	}

	ashes::DepthStencilClearValue convert( VkClearDepthStencilValue const & value )
	{
		return ashes::DepthStencilClearValue{ value.depth, value.stencil };
	}
}
