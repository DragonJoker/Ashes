#include "VkRendererPrerequisites.hpp"

namespace vk_renderer
{
	VkSubpassDescriptionFlags convert( ashes::SubpassDescriptionFlags const & flags )
	{
		return VkSubpassDescriptionFlags{ 0 };
	}

	ashes::SubpassDescriptionFlags convertSubpassDescriptionFlags( VkSubpassDescriptionFlags const & flags )
	{
		return ashes::SubpassDescriptionFlags{ 0 };
	}
}
