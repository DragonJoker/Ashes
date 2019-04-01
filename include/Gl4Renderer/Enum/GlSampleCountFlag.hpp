/*
This file belongs to Ashes.
See LICENSE file in root folder.
*/
#pragma once

#include <Ashes/Enum/SampleCountFlag.hpp>

namespace gl_renderer
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
