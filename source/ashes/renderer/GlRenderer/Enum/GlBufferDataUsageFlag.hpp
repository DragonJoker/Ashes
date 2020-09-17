/*
This file belongs to Ashes.
See LICENSE file in root folder
*/
#pragma once

namespace ashes::gl
{
	enum GlBufferDataUsageFlags
		: GLenum
	{
		GL_BUFFER_DATA_USAGE_STREAM_DRAW = 0x88E0,
		GL_BUFFER_DATA_USAGE_STREAM_READ = 0x88E1,
		GL_BUFFER_DATA_USAGE_STREAM_COPY = 0x88E2,
		GL_BUFFER_DATA_USAGE_STATIC_DRAW = 0x88E4,
		GL_BUFFER_DATA_USAGE_STATIC_READ = 0x88E5,
		GL_BUFFER_DATA_USAGE_STATIC_COPY = 0x88E6,
		GL_BUFFER_DATA_USAGE_DYNAMIC_DRAW = 0x88E8,
		GL_BUFFER_DATA_USAGE_DYNAMIC_READ = 0x88E9,
		GL_BUFFER_DATA_USAGE_DYNAMIC_COPY = 0x88EA,
	};
	std::string getName( GlBufferDataUsageFlags value );
	inline std::string toString( GlBufferDataUsageFlags value ) { return getName( value ); }
	/**
	*\brief
	*	Convertit un VkMemoryPropertyFlags en GlMemoryPropertyFlags.
	*\param[in] format
	*	Le VkMemoryPropertyFlags.
	*\return
	*	Le GlMemoryPropertyFlags.
	*/
	GlBufferDataUsageFlags getBufferDataUsageFlags( VkMemoryPropertyFlags const & flags );
}
