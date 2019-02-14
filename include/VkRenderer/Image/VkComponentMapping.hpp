/*
This file belongs to Ashes.
See LICENSE file in root folder.
*/
#ifndef ___VkRenderer_ComponentMapping_HPP___
#define ___VkRenderer_ComponentMapping_HPP___
#pragma once

#include <Ashes/Image/ComponentMapping.hpp>

namespace vk_renderer
{
	/**
	*\brief
	*	Convertit un ashes::ComponentMapping en VkComponentMapping.
	*\param[in] value
	*	Le ashes::ComponentMapping.
	*\return
	*	Le VkImageSubresourceLayers.
	*/
	VkComponentMapping convert( ashes::ComponentMapping const & value );
}

#endif
