/*
This file belongs to Ashes.
See LICENSE file in root folder
*/
#pragma once

namespace ashes::gl
{
	enum GlMemoryMapFlag
		: GLuint
	{
		GL_MEMORY_MAP_READ_BIT = 0x0001,
		GL_MEMORY_MAP_WRITE_BIT = 0x0002,
		GL_MEMORY_MAP_INVALIDATE_RANGE_BIT = 0x0004,
		GL_MEMORY_MAP_INVALIDATE_BUFFER_BIT = 0x0008,
		GL_MEMORY_MAP_FLUSH_EXPLICIT_BIT = 0x0010,
		GL_MEMORY_MAP_UNSYNCHRONIZED_BIT = 0x0020,
		GL_MEMORY_MAP_PERSISTENT_BIT = 0x00000040,
		GL_MEMORY_MAP_COHERENT_BIT = 0x00000080,
	};
	Ashes_ImplementFlag( GlMemoryMapFlag )
	std::string getName( GlMemoryMapFlags value );
	inline std::string toString( GlMemoryMapFlags value ) { return getName( value ); }
	/**
	*\brief
	*	Convertit un VkMemoryMapFlags en GlMemoryMapFlags.
	*\param[in] format
	*	Le VkMemoryMapFlags.
	*\return
	*	Le GlMemoryMapFlags.
	*/
	GlMemoryMapFlags convertMemoryMapFlags( VkMemoryPropertyFlags flags );
}
