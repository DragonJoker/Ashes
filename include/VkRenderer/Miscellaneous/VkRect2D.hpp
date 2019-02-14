/*
This file belongs to Ashes.
See LICENSE file in root folder.
*/
#ifndef ___VkRenderer_Rect2D_HPP___
#define ___VkRenderer_Rect2D_HPP___
#pragma once

#include <Ashes/Miscellaneous/Rect2D.hpp>

namespace vk_renderer
{
	/**
	*\brief
	*	Convertit un ashes::Rect2D en VkRect2D.
	*\param[in] mode
	*	Le ashes::Rect2D.
	*\return
	*	Le VkRect2D.
	*/
	VkRect2D convert( ashes::Rect2D const & value );
	/**
	*\brief
	*	Convertit un VkRect2D en ashes::Rect2D.
	*\param[in] mode
	*	Le VkRect2D.
	*\return
	*	Le ashes::Rect2D.
	*/
	ashes::Rect2D convert( VkRect2D const & value );
}

#endif
