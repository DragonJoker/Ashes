/*
This file belongs to Ashes.
See LICENSE file in root folder.
*/
#ifndef ___VkRenderer_MemoryRequirements_HPP___
#define ___VkRenderer_MemoryRequirements_HPP___
#pragma once

#include <Miscellaneous/MemoryRequirements.hpp>

namespace vk_renderer
{
	/**
	*\brief
	*	Convertit un ashes::MemoryRequirements en VkMemoryRequirements.
	*\param[in] mode
	*	Le ashes::MemoryRequirements.
	*\return
	*	Le VkMemoryRequirements.
	*/
	VkMemoryRequirements convert( ashes::MemoryRequirements const & value );
	/**
	*\brief
	*	Convertit un VkMemoryRequirements en ashes::MemoryRequirements.
	*\param[in] mode
	*	Le VkMemoryRequirements.
	*\return
	*	Le ashes::MemoryRequirements.
	*/
	ashes::MemoryRequirements convert( VkMemoryRequirements const & value );
}

#endif
