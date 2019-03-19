#include "VkRendererPrerequisites.hpp"

namespace vk_renderer
{
	VkDebugReportFlagsEXT convert( ashes::DebugReportFlags const & flags )
	{
		return VkDebugReportFlagsEXT( flags );
	}
}
