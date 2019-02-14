/*
This file belongs to Ashes.
See LICENSE file in root folder.
*/
#pragma once

#include <Ashes/Enum/DebugReportObjectType.hpp>

namespace vk_renderer
{
	/**
	*\brief
	*	Convertit un ashes::DebugReportObjectType en GlDebugReportObjectType.
	*\param[in] value
	*	Le ashes::DebugReportObjectType.
	*\return
	*	Le GlDebugReportObjectType.
	*/
	VkDebugReportObjectTypeEXT convert( ashes::DebugReportObjectType const & value );
}
