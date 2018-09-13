/*
This file belongs to Ashes.
See LICENSE file in root folder.
*/
#ifndef ___VkRenderer_LayerProperties_HPP___
#define ___VkRenderer_LayerProperties_HPP___
#pragma once

#include "VkRendererPrerequisites.hpp"

#include <Miscellaneous/LayerProperties.hpp>

namespace vk_renderer
{
	/**
	*\brief
	*	Convertit un VkLayerProperties en ashes::LayerProperties.
	*\param[in] mode
	*	Le VkLayerProperties.
	*\return
	*	Le ashes::LayerProperties.
	*/
	inline ashes::LayerProperties convert( VkLayerProperties const & props )
	{
		return ashes::LayerProperties
		{
			props.layerName,
			props.specVersion,
			props.implementationVersion,
			props.description
		};
	}
}

#endif
