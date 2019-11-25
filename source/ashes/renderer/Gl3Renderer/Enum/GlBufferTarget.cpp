#include "GlRendererPrerequisites.hpp"

namespace ashes::gl3
{
	std::string getName( GlBufferTarget value )
	{
		switch ( value )
		{
		case GL_BUFFER_TARGET_ARRAY:
			return "GL_ARRAY_BUFFER";

		case GL_BUFFER_TARGET_ELEMENT_ARRAY:
			return "GL_ELEMENT_ARRAY_BUFFER";

		case GL_BUFFER_TARGET_PIXEL_PACK:
			return "GL_PIXEL_PACK_BUFFER";

		case GL_BUFFER_TARGET_PIXEL_UNPACK:
			return "GL_PIXEL_UNPACK_BUFFER";

		case GL_BUFFER_TARGET_UNIFORM:
			return "GL_UNIFORM_BUFFER";

		case GL_BUFFER_TARGET_TEXTURE:
			return "GL_TEXTURE_BUFFER";

		case GL_BUFFER_TARGET_COPY_READ:
			return "GL_COPY_READ_BUFFER";

		case GL_BUFFER_TARGET_COPY_WRITE:
			return "GL_COPY_WRITE_BUFFER";

		case GL_BUFFER_TARGET_DRAW_INDIRECT:
			return "GL_DRAW_INDIRECT_BUFFER";

		case GL_BUFFER_TARGET_SHADER_STORAGE:
			return "GL_SHADER_STORAGE_BUFFER";

		case GL_BUFFER_TARGET_DISPATCH_INDIRECT:
			return "GL_DISPATCH_INDIRECT_BUFFER";

		case GL_BUFFER_TARGET_QUERY:
			return "GL_QUERY_BUFFER";

		default:
			assert( false && "Unsupported GlBufferTarget" );
			return "GlBufferTarget_UNKNOWN";
		}
	}

	GlBufferTarget getTargetFromUsageFlags( VkBufferUsageFlags targets )
	{
		GlBufferTarget result{ GlBufferTarget( 0 ) };

		if ( ashes::checkFlag( targets, VK_BUFFER_USAGE_UNIFORM_TEXEL_BUFFER_BIT ) )
		{
			result = GL_BUFFER_TARGET_TEXTURE;
		}
		else if ( ashes::checkFlag( targets, VK_BUFFER_USAGE_STORAGE_TEXEL_BUFFER_BIT ) )
		{
			result = GL_BUFFER_TARGET_TEXTURE;
		}
		else if ( ashes::checkFlag( targets, VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT ) )
		{
			result = GL_BUFFER_TARGET_UNIFORM;
		}
		else if ( ashes::checkFlag( targets, VK_BUFFER_USAGE_STORAGE_BUFFER_BIT ) )
		{
			result = GL_BUFFER_TARGET_SHADER_STORAGE;
		}
		else if ( ashes::checkFlag( targets, VK_BUFFER_USAGE_INDEX_BUFFER_BIT ) )
		{
			result = GL_BUFFER_TARGET_ELEMENT_ARRAY;
		}
		else if ( ashes::checkFlag( targets, VK_BUFFER_USAGE_VERTEX_BUFFER_BIT ) )
		{
			result = GL_BUFFER_TARGET_ARRAY;
		}
		else if ( ashes::checkFlag( targets, VK_BUFFER_USAGE_INDIRECT_BUFFER_BIT ) )
		{
			result = GL_BUFFER_TARGET_DISPATCH_INDIRECT;
		}
		else if ( ashes::checkFlag( targets, VK_BUFFER_USAGE_INDIRECT_BUFFER_BIT ) )
		{
			result = GL_BUFFER_TARGET_DRAW_INDIRECT;
		}
		else if ( ashes::checkFlag( targets, VK_BUFFER_USAGE_TRANSFER_SRC_BIT ) )
		{
			result = GL_BUFFER_TARGET_PIXEL_PACK;
		}
		else if ( ashes::checkFlag( targets, VK_BUFFER_USAGE_TRANSFER_DST_BIT ) )
		{
			result = GL_BUFFER_TARGET_PIXEL_UNPACK;
		}
		else if ( ashes::checkFlag( targets, VK_BUFFER_USAGE_TRANSFER_SRC_BIT ) )
		{
			result = GL_BUFFER_TARGET_COPY_READ;
		}
		else if ( ashes::checkFlag( targets, VK_BUFFER_USAGE_TRANSFER_DST_BIT ) )
		{
			result = GL_BUFFER_TARGET_COPY_WRITE;
		}

		return result;
	}
}
