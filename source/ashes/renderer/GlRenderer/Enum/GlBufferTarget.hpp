/*
This file belongs to Ashes.
See LICENSE file in root folder
*/
#pragma once

namespace ashes::gl
{
	enum GlBufferTarget
		: GLenum
	{
		GL_BUFFER_TARGET_ARRAY = 0x8892,
		GL_BUFFER_TARGET_ELEMENT_ARRAY = 0x8893,
		GL_BUFFER_TARGET_PIXEL_PACK = 0x88EB,
		GL_BUFFER_TARGET_PIXEL_UNPACK = 0x88EC,
		GL_BUFFER_TARGET_UNIFORM = 0x8A11,
		GL_BUFFER_TARGET_TEXTURE = 0x8C2A,
		GL_BUFFER_TARGET_COPY_READ = 0x8F36,
		GL_BUFFER_TARGET_COPY_WRITE = 0x8F37,
		GL_BUFFER_TARGET_DRAW_INDIRECT = 0x8F3F,
		GL_BUFFER_TARGET_SHADER_STORAGE = 0x90D2,
		GL_BUFFER_TARGET_DISPATCH_INDIRECT = 0x90EE,
		GL_BUFFER_TARGET_QUERY = 0x9192,
	};
	std::string getName( GlBufferTarget value );
	inline std::string toString( GlBufferTarget value ) { return getName( value ); }

	/**
	*\brief
	*	Convertit un VkBufferUsageFlags en GlBufferTarget.
	*\param[in] targets
	*	Le VkBufferUsageFlags.
	*\return
	*	Le GlBufferTarget.
	*/
	GlBufferTarget getTargetFromUsageFlags( VkBufferUsageFlags targets );
}
