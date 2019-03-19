/*
This file belongs to Ashes.
See LICENSE file in root folder
*/
#pragma once

#include <Ashes/AshesPrerequisites.hpp>

namespace vk_renderer
{
	/**
	*\brief
	*	Convertit un ashes::DebugReportFlags en VkDebugReportFlagsEXT.
	*\param[in] flags
	*	Le ashes::DebugReportFlags.
	*\return
	*	Le VkDebugReportFlagsEXT.
	*/
	VkDebugReportFlagsEXT convert( ashes::DebugReportFlags const & flags );
}
