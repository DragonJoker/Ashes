/*
This file belongs to Ashes.
See LICENSE file in root folder.
*/
#pragma once

#include <Ashes/Enum/MipmapMode.hpp>

namespace gl_renderer
{
	/**
	*\brief
	*	Convertit un VkSamplerMipmapMode en GlFilter.
	*\param[in] mode
	*	Le VkSamplerMipmapMode.
	*\return
	*	Le GlFilter.
	*/
	GLenum convert( VkSamplerMipmapMode const & mode );
}
