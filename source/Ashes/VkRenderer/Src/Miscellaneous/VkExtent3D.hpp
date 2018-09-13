/*
This file belongs to Ashes.
See LICENSE file in root folder.
*/
#ifndef ___VkRenderer_Extent3D_HPP___
#define ___VkRenderer_Extent3D_HPP___
#pragma once

#include <Miscellaneous/Extent3D.hpp>

namespace vk_renderer
{
	/**
	*\brief
	*	Convertit un ashes::Extent3D en VkExtent3D.
	*\param[in] mode
	*	Le ashes::Extent3D.
	*\return
	*	Le VkExtent3D.
	*/
	VkExtent3D convert( ashes::Extent3D const & value );
}

#endif
