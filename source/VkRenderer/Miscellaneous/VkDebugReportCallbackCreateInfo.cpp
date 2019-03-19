#include "VkRendererPrerequisites.hpp"

namespace vk_renderer
{
	VkDebugReportCallbackCreateInfoEXT convert( ashes::DebugReportCallbackCreateInfo const & value )
	{
		return VkDebugReportCallbackCreateInfoEXT
		{
			VK_STRUCTURE_TYPE_DEBUG_REPORT_CALLBACK_CREATE_INFO_EXT,
			nullptr,
			convert( value.flags ),
			( PFN_vkDebugReportCallbackEXT )value.pfnCallback,
			value.pUserData,
		};
	}
}
