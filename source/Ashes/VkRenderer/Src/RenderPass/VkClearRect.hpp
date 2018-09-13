/*
This file belongs to Ashes.
See LICENSE file in root folder.
*/
#ifndef ___VkRenderer_ClearRect_HPP___
#define ___VkRenderer_ClearRect_HPP___
#pragma once

#include <RenderPass/ClearRect.hpp>

namespace vk_renderer
{
	/**
	*\brief
	*	Convertit un ashes::ClearRect en VkClearRect.
	*\param[in] mode
	*	Le ashes::ClearRect.
	*\return
	*	Le VkClearRect.
	*/
	VkClearRect convert( ashes::ClearRect const & value );
}

#endif
