/*
This file belongs to Ashes.
See LICENSE file in root folder.
*/
#ifndef ___VkRenderer_BorderColour_HPP___
#define ___VkRenderer_BorderColour_HPP___
#pragma once

#include <AshesPrerequisites.hpp>

namespace vk_renderer
{
	/**
	*\brief
	*	Convertit un ashes::BorderColour en VkBorderColor.
	*\param[in] value
	*	Le ashes::BorderColour.
	*\return
	*	Le VkBorderColor.
	*/
	VkBorderColor convert( ashes::BorderColour const & value )noexcept;
}

#endif
