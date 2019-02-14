#include "VkRendererPrerequisites.hpp"

namespace vk_renderer
{
	VkDebugReportObjectTypeEXT convert( ashes::DebugReportObjectType const & value )
	{
		return VkDebugReportObjectTypeEXT( value );
	}
}
