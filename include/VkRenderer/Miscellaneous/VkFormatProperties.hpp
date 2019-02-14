/*
This file belongs to Ashes.
See LICENSE file in root folder.
*/
#ifndef ___VkRenderer_FormatProperties_HPP___
#define ___VkRenderer_FormatProperties_HPP___
#pragma once

#include "VkRenderer/VkRendererPrerequisites.hpp"

#include <Ashes/Miscellaneous/FormatProperties.hpp>

namespace vk_renderer
{
	/**
	*\brief
	*	Convertit un VkFormatProperties en ashes::FormatProperties.
	*\param[in] mode
	*	Le VkFormatProperties.
	*\return
	*	Le ashes::FormatProperties.
	*/
	inline ashes::FormatProperties convert( VkFormatProperties const & props )
	{
		return ashes::FormatProperties
		{
			convertFormatFeatureFlags( props.linearTilingFeatures ),
			convertFormatFeatureFlags( props.optimalTilingFeatures ),
			convertFormatFeatureFlags( props.bufferFeatures ),
		};
	}
}

#endif
