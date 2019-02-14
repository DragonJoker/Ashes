/*
This file belongs to Ashes.
See LICENSE file in root folder
*/
#pragma once

#include <Ashes/Enum/AccessFlag.hpp>

namespace gl_renderer
{
	enum GlAccessType
		: GLenum
	{
		GL_ACCESS_TYPE_READ_ONLY = 0x88B8,
		GL_ACCESS_TYPE_WRITE_ONLY = 0x88B9,
		GL_ACCESS_TYPE_READ_WRITE = 0x88BA,
	};
	std::string getName( GlAccessType value );
	/**
	*\brief
	*	Convertit un ashes::AccessFlags en VkAccessFlags.
	*\param[in] flags
	*	Le ashes::AccessFlags.
	*\return
	*	Le VkAccessFlags.
	*/
	GLenum convert( ashes::AccessFlags const & flags );
	/**
	*\brief
	*	Convertit un VkAccessFlags en ashes::AccessFlags.
	*\param[in] flags
	*	Le VkAccessFlags.
	*\return
	*	Le ashes::AccessFlags.
	*/
	ashes::AccessFlags convertAccessFlags( GLenum flags );
}
