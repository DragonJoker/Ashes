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
	*	Convertit un ashes::SampleCountFlag en VkSampleCountFlagBits.
	*\param[in] flags
	*	Le ashes::SampleCountFlag.
	*\return
	*	Le VkSampleCountFlagBits.
	*/
	GLsizei convert( ashes::SampleCountFlag const & flags );
}
