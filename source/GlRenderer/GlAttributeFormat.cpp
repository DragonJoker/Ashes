#include "GlAttributeFormat.hpp"

namespace gl_renderer
{
	GLenum convert( renderer::AttributeFormat format )
	{
		switch ( format )
		{
		case renderer::AttributeFormat::eFloat:
		case renderer::AttributeFormat::eVec2f:
		case renderer::AttributeFormat::eVec3f:
		case renderer::AttributeFormat::eVec4f:
			return GL_FLOAT;

		case renderer::AttributeFormat::eInt:
		case renderer::AttributeFormat::eVec2i:
		case renderer::AttributeFormat::eVec3i:
		case renderer::AttributeFormat::eVec4i:
			return GL_INT;

		default:
			assert( false && "Unsupported attribute format." );
			return GL_FLOAT;
		}
	}

	uint32_t getSize( renderer::AttributeFormat format )
	{
		switch ( format )
		{
		case renderer::AttributeFormat::eFloat:
		case renderer::AttributeFormat::eInt:
			return 1u;

		case renderer::AttributeFormat::eVec2f:
		case renderer::AttributeFormat::eVec2i:
			return 2u;

		case renderer::AttributeFormat::eVec3f:
		case renderer::AttributeFormat::eVec3i:
			return 3u;

		case renderer::AttributeFormat::eVec4f:
		case renderer::AttributeFormat::eVec4i:
			return 4u;

		default:
			assert( false && "Unsupported attribute format." );
			return 4u;
		}
	}
}
