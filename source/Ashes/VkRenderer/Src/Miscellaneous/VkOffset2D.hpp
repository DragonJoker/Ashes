/*
This file belongs to Ashes.
See LICENSE file in root folder.
*/
#ifndef ___VkRenderer_Offset2D_HPP___
#define ___VkRenderer_Offset2D_HPP___
#pragma once

#include <Miscellaneous/Offset2D.hpp>

namespace vk_renderer
{
	/**
	*\brief
	*	Convertit un ashes::Offset2D en VkOffset2D.
	*\param[in] mode
	*	Le ashes::Offset2D.
	*\return
	*	Le VkOffset2D.
	*/
	VkOffset2D convert( ashes::Offset2D const & value );
	/**
	*\brief
	*	Convertit un VkOffset2D en ashes::Offset2D.
	*\param[in] mode
	*	Le VkOffset2D.
	*\return
	*	Le ashes::Offset2D.
	*/
	ashes::Offset2D convert( VkOffset2D const & value );
}

#endif
