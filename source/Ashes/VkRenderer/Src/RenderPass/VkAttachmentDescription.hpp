/*
This file belongs to Ashes.
See LICENSE file in root folder.
*/
#ifndef ___VkRenderer_RenderPassAttachment_HPP___
#define ___VkRenderer_RenderPassAttachment_HPP___
#pragma once

#include <RenderPass/AttachmentDescription.hpp>

namespace vk_renderer
{
	/**
	*\brief
	*	Convertit un ashes::SubpassDescription en VkSubpassDescription.
	*\param[in] mode
	*	Le ashes::SubpassDescription.
	*\return
	*	Le VkSubpassDescription.
	*/
	VkAttachmentDescription convert( ashes::AttachmentDescription const & value );
}

#endif
