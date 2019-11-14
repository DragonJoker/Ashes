/*
This file belongs to Ashes.
See LICENSE file in root folder.
*/
#pragma once

namespace ashes::gl3
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
