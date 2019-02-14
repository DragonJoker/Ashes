/*
This file belongs to Ashes.
See LICENSE file in root folder
*/
#pragma once

#include <Ashes/Enum/MemoryPropertyFlag.hpp>

namespace gl_renderer
{
	enum GlMemoryPropertyFlag
		: GLuint
	{
		GL_MEMORY_PROPERTY_READ_BIT = 0x0001,
		GL_MEMORY_PROPERTY_WRITE_BIT = 0x0002,
		GL_MEMORY_PROPERTY_PERSISTENT_BIT = 0x00000040,
		GL_MEMORY_PROPERTY_COHERENT_BIT = 0x00000080,
		GL_MEMORY_PROPERTY_DYNAMIC_STORAGE_BIT = 0x0100,
	};
	Ashes_ImplementFlag( GlMemoryPropertyFlag )
	std::string getName( GlMemoryPropertyFlags value );
	/**
	*\brief
	*	Convertit un ashes::MemoryPropertyFlags en GlMemoryPropertyFlags.
	*\param[in] format
	*	Le ashes::MemoryPropertyFlags.
	*\return
	*	Le GlMemoryPropertyFlags.
	*/
	GlMemoryPropertyFlags convert( ashes::MemoryPropertyFlags const & flags );
}
