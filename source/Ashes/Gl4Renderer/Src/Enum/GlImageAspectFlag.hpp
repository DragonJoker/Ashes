/*
This file belongs to Ashes.
See LICENSE file in root folder
*/
#pragma once

#include <Enum/ImageAspectFlag.hpp>

namespace gl_renderer
{
	enum GlImageAspectFlag
		: GLenum
	{
		GL_DEPTH_BUFFER_BIT = 0x00000100,
		GL_STENCIL_BUFFER_BIT = 0x00000400,
		GL_COLOR_BUFFER_BIT = 0x00004000,
	};
	Ashes_ImplementFlag( GlImageAspectFlag )
	std::string getName( GlImageAspectFlags value );
	/**
	*\brief
	*	Convertit un ashes::ImageAspectFlags en GlImageAspectFlags.
	*\param[in] flags
	*	Le ashes::ImageAspectFlags.
	*\return
	*	Le GlImageAspectFlags.
	*/
	GlImageAspectFlags convert( ashes::ImageAspectFlags const & flags );
}
