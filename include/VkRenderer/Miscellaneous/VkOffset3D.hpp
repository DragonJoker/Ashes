/*
This file belongs to Ashes.
See LICENSE file in root folder.
*/
#ifndef ___VkRenderer_Offset3D_HPP___
#define ___VkRenderer_Offset3D_HPP___
#pragma once

#include <Ashes/Miscellaneous/Offset3D.hpp>

namespace vk_renderer
{
	/**
	*\brief
	*	Convertit un ashes::Offset3D en VkOffset3D.
	*\param[in] mode
	*	Le ashes::Offset3D.
	*\return
	*	Le VkOffset3D.
	*/
	VkOffset3D convert( ashes::Offset3D const & value );
}

#endif
