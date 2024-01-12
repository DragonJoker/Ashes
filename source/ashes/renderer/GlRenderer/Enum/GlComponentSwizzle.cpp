#include "GlRendererPrerequisites.hpp"

namespace ashes::gl
{
	std::string getName( GlSwizzle value )
	{
		switch ( value )
		{
		case GL_SWIZZLE_R:
			return "GL_TEXTURE_SWIZZLE_R";

		case GL_SWIZZLE_G:
			return "GL_TEXTURE_SWIZZLE_G";

		case GL_SWIZZLE_B:
			return "GL_TEXTURE_SWIZZLE_B";

		case GL_SWIZZLE_A:
			return "GL_TEXTURE_SWIZZLE_A";

		default:
			assert( false && "Unsupported GlSwizzle." );
			return "GlSwizzle_UNKNOWN";
		}
	}

	std::string getName( GlComponentSwizzle value )
	{
		switch ( value )
		{
		case GL_COMPONENT_SWIZZLE_ZERO:
			return "GL_ZERO";

		case GL_COMPONENT_SWIZZLE_ONE:
			return "GL_ONE";

		case GL_COMPONENT_SWIZZLE_RED:
			return "GL_RED";

		case GL_COMPONENT_SWIZZLE_GREEN:
			return "GL_GREEN";

		case GL_COMPONENT_SWIZZLE_BLUE:
			return "GL_BLUE";

		case GL_COMPONENT_SWIZZLE_ALPHA:
			return "GL_ALPHA";

		default:
			assert( false && "Unsupported GlComponentSwizzle." );
			return "GlComponentSwizzle_UNKNOWN";
		}
	}

	GlComponentSwizzle convertComponentSwizzle( VkComponentSwizzle const & value )
	{
		switch ( value )
		{
		case VK_COMPONENT_SWIZZLE_IDENTITY:
			return GL_COMPONENT_SWIZZLE_IDENTITY;

		case VK_COMPONENT_SWIZZLE_ZERO:
			return GL_COMPONENT_SWIZZLE_ZERO;

		case VK_COMPONENT_SWIZZLE_ONE:
			return GL_COMPONENT_SWIZZLE_ONE;

		case VK_COMPONENT_SWIZZLE_R:
			return GL_COMPONENT_SWIZZLE_RED;

		case VK_COMPONENT_SWIZZLE_G:
			return GL_COMPONENT_SWIZZLE_GREEN;

		case VK_COMPONENT_SWIZZLE_B:
			return GL_COMPONENT_SWIZZLE_BLUE;

		case VK_COMPONENT_SWIZZLE_A:
			return GL_COMPONENT_SWIZZLE_ALPHA;

		default:
			assert( false && "Unsupported ComponentSwizzle." );
			return GL_COMPONENT_SWIZZLE_IDENTITY;
		}
	}

	bool operator==( GlComponentMapping const & lhs, GlComponentMapping const & rhs )
	{
		return lhs.r == rhs.r
			&& lhs.g == rhs.g
			&& lhs.b == rhs.b
			&& lhs.a == rhs.a;
	}

	static GlComponentMapping const SwizzleIdentity
	{
		GlComponentSwizzle::GL_COMPONENT_SWIZZLE_RED,
		GlComponentSwizzle::GL_COMPONENT_SWIZZLE_GREEN,
		GlComponentSwizzle::GL_COMPONENT_SWIZZLE_BLUE,
		GlComponentSwizzle::GL_COMPONENT_SWIZZLE_ALPHA,
	};

	GlComponentMapping const & getIdentitySwizzle()
	{
		return SwizzleIdentity;
	}

	bool isIdentity( GlComponentMapping const & value )
	{
		return value == getIdentitySwizzle();
	}
}
