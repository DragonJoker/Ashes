/*
This file belongs to Ashes.
See LICENSE file in root folder.
*/
#pragma once

namespace ashes::gl
{
	enum GlWrapMode
		: GLenum
	{
		GL_WRAP_MODE_REPEAT = 0x2901,
		GL_WRAP_MODE_MIRRORED_REPEAT = 0x8370,
		GL_WRAP_MODE_CLAMP_TO_EDGE = 0x812F,
		GL_WRAP_MODE_CLAMP_TO_BORDER = 0x812D,
		GL_WRAP_MODE_MIRROR_CLAMP_TO_EDGE = 0x8743,
	};
	std::string getName( GlWrapMode value );
	inline std::string toString( GlWrapMode value ) { return getName( value ); }
	/**
	*\brief
	*	Convertit un VkSamplerAddressMode en GlWrapMode.
	*\param[in] mode
	*	Le VkSamplerAddressMode.
	*\return
	*	Le GlWrapMode.
	*/
	GlWrapMode convert( VkSamplerAddressMode const & mode );
}
