/*
This file belongs to RendererLib.
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
	*	Convertit un VkLayerProperties en renderer::LayerProperties.
	*\param[in] mode
	*	Le VkLayerProperties.
	*\return
	*	Le renderer::LayerProperties.
	*/
	inline renderer::LayerProperties convert( VkLayerProperties const & props )
	{
		return renderer::LayerProperties
		{
			props.layerName,
			props.specVersion,
			props.implementationVersion,
			props.description
		};
	}
}

#endif
