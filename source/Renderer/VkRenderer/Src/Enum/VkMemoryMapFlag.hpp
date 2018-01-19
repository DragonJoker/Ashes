/*
This file belongs to Renderer.
See LICENSE file in root folder
*/
#pragma once

#include <Enum/MemoryMapFlag.hpp>

namespace vk_renderer
{
	/**
	*\brief
	*	Convertit un renderer::MemoryMapFlags en VkMemoryMapFlags.
	*\param[in] format
	*	Le renderer::MemoryMapFlags.
	*\return
	*	Le VkMemoryMapFlags.
	*/
	VkMemoryMapFlags convert( renderer::MemoryMapFlags const & flags );
}
