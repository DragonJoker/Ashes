#include "GlRendererPrerequisites.hpp"

namespace gl_renderer
{
	GLenum convert( renderer::BufferTargets const & targets )
	{
		GLenum result{ 0 };

		if ( checkFlag( targets, renderer::BufferTarget::eIndexBuffer ) )
		{
			result = GL_ELEMENT_ARRAY_BUFFER;
		}
		else if ( checkFlag( targets, renderer::BufferTarget::eDispatchIndirectBuffer ) )
		{
			result = GL_DISPATCH_INDIRECT_BUFFER;
		}
		else if ( checkFlag( targets, renderer::BufferTarget::eStorageBuffer ) )
		{
			result = GL_SHADER_STORAGE_BUFFER;
		}
		else if ( checkFlag( targets, renderer::BufferTarget::eStorageTexelBuffer ) )
		{
			result = GL_TEXTURE_BUFFER;
		}
		else if ( checkFlag( targets, renderer::BufferTarget::eTransferDst ) )
		{
			result = GL_COPY_WRITE_BUFFER;
		}
		else if ( checkFlag( targets, renderer::BufferTarget::eTransferSrc ) )
		{
			result = GL_COPY_READ_BUFFER;
		}
		else if ( checkFlag( targets, renderer::BufferTarget::eUniformBuffer ) )
		{
			result = GL_UNIFORM_BUFFER;
		}
		else if ( checkFlag( targets, renderer::BufferTarget::eUniformTexelBuffer ) )
		{
			result = GL_TEXTURE_BUFFER;
		}
		else if ( checkFlag( targets, renderer::BufferTarget::eVertexBuffer ) )
		{
			result = GL_ARRAY_BUFFER;
		}
		else if ( checkFlag( targets, renderer::BufferTarget::eDrawIndirectBuffer ) )
		{
			result = GL_DRAW_INDIRECT_BUFFER;
		}
		else if ( checkFlag( targets, renderer::BufferTarget::ePackBuffer ) )
		{
			result = GL_PIXEL_PACK_BUFFER;
		}
		else if ( checkFlag( targets, renderer::BufferTarget::eUnpackBuffer ) )
		{
			result = GL_PIXEL_UNPACK_BUFFER;
		}

		return result;
	}
}
