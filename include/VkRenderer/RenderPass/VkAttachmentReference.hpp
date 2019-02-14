/*
This file belongs to Ashes.
See LICENSE file in root folder.
*/
#ifndef ___VkRenderer_RenderSubpassAttachment_HPP___
#define ___VkRenderer_RenderSubpassAttachment_HPP___
#pragma once

#include <Ashes/RenderPass/AttachmentDescription.hpp>

namespace vk_renderer
{
	/**
	*\brief
	*	Convertit un ashes::AttachmentReference en VkAttachmentReference.
	*\param[in] mode
	*	Le ashes::AttachmentReference.
	*\return
	*	Le VkAttachmentReference.
	*/
	VkAttachmentReference convert( ashes::AttachmentReference const & value );
}

#endif
