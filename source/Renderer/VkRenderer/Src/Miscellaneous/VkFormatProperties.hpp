/*
This file belongs to RendererLib.
See LICENSE file in root folder.
*/
#ifndef ___VkRenderer_FormatProperties_HPP___
#define ___VkRenderer_FormatProperties_HPP___
#pragma once

#include "VkRendererPrerequisites.hpp"

#include <Miscellaneous/FormatProperties.hpp>

namespace vk_renderer
{
	/**
	*\brief
	*	Convertit un VkFormatProperties en renderer::FormatProperties.
	*\param[in] mode
	*	Le VkFormatProperties.
	*\return
	*	Le renderer::FormatProperties.
	*/
	inline renderer::FormatProperties convert( VkFormatProperties const & props )
	{
		return renderer::FormatProperties
		{
			convertFormatFeatureFlags( props.linearTilingFeatures ),
			convertFormatFeatureFlags( props.optimalTilingFeatures ),
			convertFormatFeatureFlags( props.bufferFeatures ),
		};
	}
}

#endif
