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
	*	Le ashes::Extent3D.
	*\return
	*	Le VkOffset2D.
	*/
	VkOffset2D convert( ashes::Offset2D const & value );
}

#endif
