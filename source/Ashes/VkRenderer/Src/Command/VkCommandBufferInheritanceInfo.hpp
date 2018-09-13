/*
This file belongs to Ashes.
See LICENSE file in root folder.
*/
#ifndef ___VkRenderer_CommandBufferInheritanceInfo_HPP___
#define ___VkRenderer_CommandBufferInheritanceInfo_HPP___
#pragma once

#include <Command/CommandBufferInheritanceInfo.hpp>

namespace vk_renderer
{
	/**
	*\brief
	*	Convertit un ashes::CommandBufferInheritanceInfo en VkCommandBufferInheritanceInfo.
	*\param[in] mode
	*	Le ashes::CommandBufferInheritanceInfo.
	*\return
	*	Le VkCommandBufferInheritanceInfo.
	*/
	VkCommandBufferInheritanceInfo convert( ashes::CommandBufferInheritanceInfo const & value );
}

#endif
