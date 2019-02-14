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
	*	Convertit un ashes::MipmapMode en GlFilter.
	*\param[in] mode
	*	Le ashes::MipmapMode.
	*\return
	*	Le GlFilter.
	*/
	GLenum convert( ashes::MipmapMode const & mode );
}
