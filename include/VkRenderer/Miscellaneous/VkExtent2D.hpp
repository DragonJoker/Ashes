/*
This file belongs to Ashes.
See LICENSE file in root folder.
*/
#ifndef ___VkRenderer_Extent2D_HPP___
#define ___VkRenderer_Extent2D_HPP___
#pragma once

#include <Ashes/Miscellaneous/Extent2D.hpp>

namespace vk_renderer
{
	/**
	*\brief
	*	Convertit un ashes::Extent2D en VkExtent2D.
	*\param[in] mode
	*	Le ashes::Extent2D.
	*\return
	*	Le VkExtent2D.
	*/
	VkExtent2D convert( ashes::Extent2D const & value );
	/**
	*\brief
	*	Convertit un VkExtent2D en ashes::Extent2D.
	*\param[in] mode
	*	Le VkExtent2D.
	*\return
	*	Le ashes::Extent2D.
	*/
	ashes::Extent2D convert( VkExtent2D const & value );
}

#endif
