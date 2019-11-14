/*
This file belongs to Ashes.
See LICENSE file in root folder.
*/
#pragma once

namespace ashes::gl3
{
	/**
	*\brief
	*	Convertit un VkSampleCountFlagBits en VkSampleCountFlagBits.
	*\param[in] flags
	*	Le VkSampleCountFlagBits.
	*\return
	*	Le VkSampleCountFlagBits.
	*/
	GLsizei convert( VkSampleCountFlagBits const & flags );
}
