/*
This file belongs to Ashes.
See LICENSE file in root folder
*/
#pragma once

namespace ashes::gl
{
	enum GlCullMode
		: GLenum
	{
		GL_CULL_MODE_NONE = 0x0000,
		GL_CULL_MODE_FRONT = 0x0404,
		GL_CULL_MODE_BACK = 0x0405,
		GL_CULL_MODE_FRONT_AND_BACK = 0x0408,
	};
	std::string getName( GlCullMode value );
	inline std::string toString( GlCullMode value ) { return getName( value ); }
	/**
	*\brief
	*	Convertit un VkCullModeFlags en GlCullMode.
	*\param[in] flags
	*	Le VkCullModeFlags.
	*\return
	*	Le GlCullMode.
	*/
	GlCullMode convertCullModeFlags( VkCullModeFlags flags );
}
