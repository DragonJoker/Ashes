#include "VkRendererPrerequisites.hpp"

namespace vk_renderer
{
	VkPushConstantRange convert( ashes::PushConstantRange const & value )
	{
		return VkPushConstantRange
		{
			convert( value.stageFlags ),
			value.offset,
			value.size,
		};
	}
}
