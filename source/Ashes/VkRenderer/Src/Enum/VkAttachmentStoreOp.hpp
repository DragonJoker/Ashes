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
	*	Convertit un ashes::AttachmentStoreOp en VkAttachmentStoreOp.
	*\param[in] value
	*	Le ashes::AttachmentStoreOp.
	*\return
	*	Le VkAttachmentStoreOp.
	*/
	VkAttachmentStoreOp convert( ashes::AttachmentStoreOp const & value );
}
