#include "RendererPrerequisites.hpp"

#include <Utils/Vec4.hpp>

namespace renderer
{
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
}
