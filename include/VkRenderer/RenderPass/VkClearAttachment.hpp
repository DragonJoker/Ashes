/*
This file belongs to Ashes.
See LICENSE file in root folder.
*/
#ifndef ___VkRenderer_ClearAttachment_HPP___
#define ___VkRenderer_ClearAttachment_HPP___
#pragma once

#include <Ashes/RenderPass/ClearAttachment.hpp>

namespace vk_renderer
{
	/**
	*\brief
	*	Convertit un ashes::ClearAttachment en VkClearAttachment.
	*\param[in] mode
	*	Le ashes::ClearAttachment.
	*\return
	*	Le VkClearAttachment.
	*/
	VkClearAttachment convert( ashes::ClearAttachment const & value );
}

#endif
