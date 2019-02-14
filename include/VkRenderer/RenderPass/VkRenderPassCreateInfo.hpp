/*
This file belongs to Ashes.
See LICENSE file in root folder.
*/
#pragma once

#include <Ashes/RenderPass/RenderPassCreateInfo.hpp>

namespace vk_renderer
{
	/**
	*\brief
	*	Convertit un ashes::RenderPassCreateInfo en VkRenderPassCreateInfo.
	*\param[in] mode
	*	Le ashes::RenderPassCreateInfo.
	*\return
	*	Le VkRenderPassCreateInfo.
	*/
	VkRenderPassCreateInfo convert( ashes::RenderPassCreateInfo const & value
		, std::vector< VkAttachmentDescription > const & attachments
		, std::vector< VkSubpassDescription > const & subpasses
		, std::vector< VkSubpassDependency > const & dependencies );
}
