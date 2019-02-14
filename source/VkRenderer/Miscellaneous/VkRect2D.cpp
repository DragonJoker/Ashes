#include "VkRendererPrerequisites.hpp"

#include "Miscellaneous/VkExtent2D.hpp"
#include "Miscellaneous/VkOffset2D.hpp"

namespace vk_renderer
{
	VkRect2D convert( ashes::Rect2D const & value )
	{
		return VkRect2D
		{
			convert( value.offset ),
			convert( value.extent ),
		};
	}

	ashes::Rect2D convert( VkRect2D const & value )
	{
		return ashes::Rect2D
		{
			convert( value.offset ),
			convert( value.extent ),
		};
	}
}
