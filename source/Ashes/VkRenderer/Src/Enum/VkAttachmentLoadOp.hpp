/*
This file belongs to Ashes.
See LICENSE file in root folder
*/
#pragma once

#include <AshesPrerequisites.hpp>

namespace vk_renderer
{
	/**
	*\brief
	*	Convertit un ashes::AttachmentLoadOp en VkAttachmentLoadOp.
	*\param[in] value
	*	Le ashes::AttachmentLoadOp.
	*\return
	*	Le VkAttachmentLoadOp.
	*/
	VkAttachmentLoadOp convert( ashes::AttachmentLoadOp const & value );
}
