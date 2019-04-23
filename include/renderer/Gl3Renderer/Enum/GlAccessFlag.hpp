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
	*	Convertit un VkAccessFlags en VkAccessFlags.
	*\param[in] flags
	*	Le VkAccessFlags.
	*\return
	*	Le VkAccessFlags.
	*/
	GLenum convert( VkAccessFlags const & flags );
	/**
	*\brief
	*	Convertit un VkAccessFlags en VkAccessFlags.
	*\param[in] flags
	*	Le VkAccessFlags.
	*\return
	*	Le VkAccessFlags.
	*/
	VkAccessFlags convertAccessFlags( GLenum flags );
}
