/*
This file belongs to Ashes.
See LICENSE file in root folder
*/
#pragma once

namespace ashes::gl
{
	enum GlAccessType
		: GLenum
	{
		GL_ACCESS_TYPE_READ_ONLY = 0x88B8,
		GL_ACCESS_TYPE_WRITE_ONLY = 0x88B9,
		GL_ACCESS_TYPE_READ_WRITE = 0x88BA,
	};
	std::string getName( GlAccessType value );
	inline std::string toString( GlAccessType value ) { return getName( value ); }
	/**
	*\brief
	*	Convertit un VkAccessFlags en GLenum.
	*\param[in] flags
	*	Le VkAccessFlags.
	*\return
	*	Le GLenum.
	*/
	GLenum convertAccessFlags( VkAccessFlags const & flags );
	/**
	*\brief
	*	Convertit un GLenum en VkAccessFlags.
	*\param[in] flags
	*	Le GLenum.
	*\return
	*	Le VkAccessFlags.
	*/
	VkAccessFlags convertAccessFlags( GLenum flags );
}
