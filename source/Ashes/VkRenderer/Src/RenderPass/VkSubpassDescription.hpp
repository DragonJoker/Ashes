/*
This file belongs to Ashes.
See LICENSE file in root folder.
*/
#ifndef ___VkRenderer_SubpassDescription_HPP___
#define ___VkRenderer_SubpassDescription_HPP___
#pragma once

#include <RenderPass/SubpassDescription.hpp>

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
	VkSubpassDescription convert( ashes::SubpassDescription const & value
		, std::vector< VkAttachmentReference > const & inputAttachments
		, std::vector< VkAttachmentReference > const & colorAttachments
		, std::vector< VkAttachmentReference > const & resolveAttachments
		, VkAttachmentReference const & depthStencilAttachment );
}

#endif
