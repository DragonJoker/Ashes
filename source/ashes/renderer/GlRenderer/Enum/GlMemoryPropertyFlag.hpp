/*
This file belongs to Ashes.
See LICENSE file in root folder
*/
#pragma once

namespace ashes::gl
{
	namespace gl3
	{
		enum GlBufferMemoryPropertyFlag
			: GLuint
		{
			GL_STREAM_DRAW = 0x88E0,
			GL_STREAM_READ = 0x88E1,
			GL_STREAM_COPY = 0x88E2,
			GL_STATIC_DRAW = 0x88E4,
			GL_STATIC_READ = 0x88E5,
			GL_STATIC_COPY = 0x88E6,
			GL_DYNAMIC_DRAW = 0x88E8,
			GL_DYNAMIC_READ = 0x88E9,
			GL_DYNAMIC_COPY = 0x88EA,
		};
		std::string getName( GlBufferMemoryPropertyFlag value );
		inline std::string toString( GlBufferMemoryPropertyFlag value ) { return getName( value ); }
		GlBufferMemoryPropertyFlag getBufferMemoryPropertyFlag( VkMemoryPropertyFlags flags );
	}

	namespace gl4
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
		inline std::string toString( GlMemoryPropertyFlags value ) { return getName( value ); }
		GlMemoryPropertyFlags convertMemoryPropertyFlags( VkMemoryPropertyFlags const & flags );
	}
}
