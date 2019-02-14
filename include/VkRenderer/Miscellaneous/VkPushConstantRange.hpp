/*
This file belongs to Ashes.
See LICENSE file in root folder.
*/
#ifndef ___VkRenderer_PushConstantRange_HPP___
#define ___VkRenderer_PushConstantRange_HPP___
#pragma once

#include <Ashes/Miscellaneous/PushConstantRange.hpp>

namespace vk_renderer
{
	/**
	*\brief
	*	Convertit un ashes::PushConstantRange en VkPushConstantRange.
	*\param[in] mode
	*	Le ashes::PushConstantRange.
	*\return
	*	Le VkPushConstantRange.
	*/
	VkPushConstantRange convert( ashes::PushConstantRange const & value );
}

#endif
