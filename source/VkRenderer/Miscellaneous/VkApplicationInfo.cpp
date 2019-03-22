#include "VkRendererPrerequisites.hpp"

namespace vk_renderer
{
	VkApplicationInfo convert( ashes::ApplicationInfo const & value )
	{
		return VkApplicationInfo
		{
			VK_STRUCTURE_TYPE_APPLICATION_INFO,
			nullptr,
			value.applicationName.c_str(),
			value.applicationVersion,
			value.engineName.c_str(),
			value.engineVersion,
			value.apiVersion
		};
	}
}
