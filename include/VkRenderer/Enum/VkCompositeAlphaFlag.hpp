/*
This file belongs to Ashes.
See LICENSE file in root folder
*/
#pragma once

#include <Ashes/AshesPrerequisites.hpp>

namespace vk_renderer
{
	/**
	*\brief
	*	Convertit un ashes::CompositeAlphaFlags en VkCompositeAlphaFlagsKHR.
	*\param[in] flags
	*	Le ashes::CompositeAlphaFlags.
	*\return
	*	Le VkCompositeAlphaFlagsKHR.
	*/
	VkCompositeAlphaFlagsKHR convert( ashes::CompositeAlphaFlags const & flags );
	/**
	*\brief
	*	Convertit un ashes::CompositeAlphaFlag en VkCompositeAlphaFlagBitsKHR.
	*\param[in] flag
	*	Le ashes::CompositeAlphaFlag.
	*\return
	*	Le VkCompositeAlphaFlagBitsKHR.
	*/
	VkCompositeAlphaFlagBitsKHR convert( ashes::CompositeAlphaFlag const & flag );
	/**
	*\brief
	*	Convertit un VkCompositeAlphaFlagsKHR en ashes::CompositeAlphaFlags.
	*\param[in] flags
	*	Le VkCompositeAlphaFlagsKHR.
	*\return
	*	Le ashes::CompositeAlphaFlags.
	*/
	ashes::CompositeAlphaFlags convertCompositeAlpha( VkCompositeAlphaFlagsKHR const & flags );
}
