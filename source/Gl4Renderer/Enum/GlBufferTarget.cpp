#include "GlRendererPrerequisites.hpp"

namespace gl_renderer
{
	std::string getName( GlBufferTarget value )
	{
		switch ( value )
		{
		case gl_renderer::GL_BUFFER_TARGET_ARRAY:
			return "GL_ARRAY_BUFFER";

		case gl_renderer::GL_BUFFER_TARGET_ELEMENT_ARRAY:
			return "GL_ELEMENT_ARRAY_BUFFER";

		case gl_renderer::GL_BUFFER_TARGET_PIXEL_PACK:
			return "GL_PIXEL_PACK_BUFFER";

		case gl_renderer::GL_BUFFER_TARGET_PIXEL_UNPACK:
			return "GL_PIXEL_UNPACK_BUFFER";

		case gl_renderer::GL_BUFFER_TARGET_UNIFORM:
			return "GL_UNIFORM_BUFFER";

		case gl_renderer::GL_BUFFER_TARGET_TEXTURE:
			return "GL_TEXTURE_BUFFER";

		case gl_renderer::GL_BUFFER_TARGET_COPY_READ:
			return "GL_COPY_READ_BUFFER";

		case gl_renderer::GL_BUFFER_TARGET_COPY_WRITE:
			return "GL_COPY_WRITE_BUFFER";

		case gl_renderer::GL_BUFFER_TARGET_DRAW_INDIRECT:
			return "GL_DRAW_INDIRECT_BUFFER";

		case gl_renderer::GL_BUFFER_TARGET_SHADER_STORAGE:
			return "GL_SHADER_STORAGE_BUFFER";

		case gl_renderer::GL_BUFFER_TARGET_DISPATCH_INDIRECT:
			return "GL_DISPATCH_INDIRECT_BUFFER";

		default:
			assert( false && "Unsupported GlBufferTarget" );
			return "GlBufferTarget_UNKNOWN";
		}
	}

	uint32_t checkFlag( uint32_t a, uint32_t b )
	{
		return ( a & b ) == b;
	}

	GlBufferTarget getTargetFromUsageFlags( VkBufferUsageFlags targets )
	{
		GlBufferTarget result{ GlBufferTarget( 0 ) };

		if ( checkFlag( targets, VK_BUFFER_USAGE_UNIFORM_TEXEL_BUFFER_BIT ) )
		{
			result = GL_BUFFER_TARGET_TEXTURE;
		}
		else if ( checkFlag( targets, VK_BUFFER_USAGE_STORAGE_TEXEL_BUFFER_BIT ) )
		{
			result = GL_BUFFER_TARGET_TEXTURE;
		}
		else if ( checkFlag( targets, VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT ) )
		{
			result = GL_BUFFER_TARGET_UNIFORM;
		}
		else if ( checkFlag( targets, VK_BUFFER_USAGE_STORAGE_BUFFER_BIT ) )
		{
			result = GL_BUFFER_TARGET_SHADER_STORAGE;
		}
		else if ( checkFlag( targets, VK_BUFFER_USAGE_INDEX_BUFFER_BIT ) )
		{
			result = GL_BUFFER_TARGET_ELEMENT_ARRAY;
		}
		else if ( checkFlag( targets, VK_BUFFER_USAGE_VERTEX_BUFFER_BIT ) )
		{
			result = GL_BUFFER_TARGET_ARRAY;
		}
		else if ( checkFlag( targets, VK_BUFFER_USAGE_INDIRECT_BUFFER_BIT ) )
		{
			result = GL_BUFFER_TARGET_DISPATCH_INDIRECT;
		}
		else if ( checkFlag( targets, VK_BUFFER_USAGE_INDIRECT_BUFFER_BIT ) )
		{
			result = GL_BUFFER_TARGET_DRAW_INDIRECT;
		}
		else if ( checkFlag( targets, VK_BUFFER_USAGE_TRANSFER_SRC_BIT ) )
		{
			result = GL_BUFFER_TARGET_PIXEL_PACK;
		}
		else if ( checkFlag( targets, VK_BUFFER_USAGE_TRANSFER_DST_BIT ) )
		{
			result = GL_BUFFER_TARGET_PIXEL_UNPACK;
		}
		else if ( checkFlag( targets, VK_BUFFER_USAGE_TRANSFER_SRC_BIT ) )
		{
			result = GL_BUFFER_TARGET_COPY_READ;
		}
		else if ( checkFlag( targets, VK_BUFFER_USAGE_TRANSFER_DST_BIT ) )
		{
			result = GL_BUFFER_TARGET_COPY_WRITE;
		}

		return result;
	}
}
