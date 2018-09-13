/*
This file belongs to Ashes.
See LICENSE file in root folder
*/
#pragma once

#include <AshesPrerequisites.hpp>

namespace vk_renderer
{
	/**
	*\brief
	*	Convertit un ashes::FormatFeatureFlags en VkFormatFeatureFlags.
	*\param[in] flags
	*	Le ashes::FormatFeatureFlags.
	*\return
	*	Le VkFormatFeatureFlags.
	*/
	VkFormatFeatureFlags convert( ashes::FormatFeatureFlags const & flags );
	/**
	*\brief
	*	Convertit un VkFormatFeatureFlags en ashes::FormatFeatureFlags.
	*\param[in] flags
	*	Le VkFormatFeatureFlags.
	*\return
	*	Le ashes::FormatFeatureFlags.
	*/
	ashes::FormatFeatureFlags convertFormatFeatureFlags( VkFormatFeatureFlags const & flags );
}
