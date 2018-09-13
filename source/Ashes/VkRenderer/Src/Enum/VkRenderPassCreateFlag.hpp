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
	*	Convertit un ashes::RenderPassCreateFlags en VkRenderPassCreateFlags.
	*\param[in] flags
	*	Le ashes::RenderPassCreateFlags.
	*\return
	*	Le VkRenderPassCreateFlags.
	*/
	VkRenderPassCreateFlags convert( ashes::RenderPassCreateFlags const & flags );
	/**
	*\brief
	*	Convertit un VkRenderPassCreateFlags en ashes::RenderPassCreateFlags.
	*\param[in] flags
	*	Le VkRenderPassCreateFlags.
	*\return
	*	Le ashes::RenderPassCreateFlags.
	*/
	ashes::RenderPassCreateFlags convertRenderPassCreateFlags( VkRenderPassCreateFlags flags );
}
