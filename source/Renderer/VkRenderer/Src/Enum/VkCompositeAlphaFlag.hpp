/*
This file belongs to RendererLib.
See LICENSE file in root folder
*/
#pragma once

#include <RendererPrerequisites.hpp>

namespace vk_renderer
{
	/**
	*\brief
	*	Convertit un renderer::CompositeAlphaFlags en VkCompositeAlphaFlagsKHR.
	*\param[in] flags
	*	Le renderer::CompositeAlphaFlags.
	*\return
	*	Le VkCompositeAlphaFlagsKHR.
	*/
	VkCompositeAlphaFlagsKHR convert( renderer::CompositeAlphaFlags const & flags );
	/**
	*\brief
	*	Convertit un VkCompositeAlphaFlagsKHR en renderer::CompositeAlphaFlags.
	*\param[in] flags
	*	Le VkCompositeAlphaFlagsKHR.
	*\return
	*	Le renderer::CompositeAlphaFlags.
	*/
	renderer::CompositeAlphaFlags convertCompositeAlpha( VkCompositeAlphaFlagsKHR const & flags );
}
