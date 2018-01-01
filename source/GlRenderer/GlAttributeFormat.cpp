#include "GlAttributeFormat.hpp"

namespace gl_renderer
{
	GLenum convert( renderer::AttributeFormat format )
	{
		switch ( format )
		{
		case renderer::AttributeFormat::eFloat:
			return GL_R32F;

		case renderer::AttributeFormat::eVec2f:
			return GL_RG32F;

		case renderer::AttributeFormat::eVec3f:
			return GL_RGB32F;

		case renderer::AttributeFormat::eVec4f:
			return GL_RGBA32F;

		case renderer::AttributeFormat::eInt:
			return GL_R32I;

		case renderer::AttributeFormat::eVec2i:
			return GL_RG32I;

		case renderer::AttributeFormat::eVec3i:
			return GL_RGB32I;

		case renderer::AttributeFormat::eVec4i:
			return GL_RGBA32I;

		default:
			assert( false && "Unsupported attribute format." );
			return GL_RGBA32F;
		}
	}

	uint32_t getSize( renderer::AttributeFormat format )
	{
		switch ( format )
		{
		case renderer::AttributeFormat::eFloat:
		case renderer::AttributeFormat::eInt:
			return 4u;

		case renderer::AttributeFormat::eVec2f:
		case renderer::AttributeFormat::eVec2i:
			return 4u * 2u;

		case renderer::AttributeFormat::eVec3f:
		case renderer::AttributeFormat::eVec3i:
			return 4u * 3u;

		case renderer::AttributeFormat::eVec4f:
		case renderer::AttributeFormat::eVec4i:
			return 4u * 4u;

		default:
			assert( false && "Unsupported attribute format." );
			return 4u * 4u;
		}
	}
}
