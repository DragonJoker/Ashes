/*
This file belongs to Ashes.
See LICENSE file in root folder
*/
#pragma once

namespace ashes::gl
{
	enum GlSwizzle
		: GLenum
	{
		GL_SWIZZLE_R = 0x8E42,
		GL_SWIZZLE_G = 0x8E43,
		GL_SWIZZLE_B = 0x8E44,
		GL_SWIZZLE_A = 0x8E45,
	};
	std::string getName( GlSwizzle value );
	inline std::string toString( GlSwizzle value ) { return getName( value ); }

	enum GlComponentSwizzle
		: GLenum
	{
		GL_COMPONENT_SWIZZLE_IDENTITTY = ~GLenum( 0 ),
		GL_COMPONENT_SWIZZLE_ZERO = 0,
		GL_COMPONENT_SWIZZLE_ONE = 1,
		GL_COMPONENT_SWIZZLE_RED = 0x1903,
		GL_COMPONENT_SWIZZLE_GREEN = 0x1904,
		GL_COMPONENT_SWIZZLE_BLUE = 0x1905,
		GL_COMPONENT_SWIZZLE_ALPHA = 0x1906,
	};
	std::string getName( GlComponentSwizzle value );
	inline std::string toString( GlComponentSwizzle value ) { return getName( value ); }
	GlComponentSwizzle convertComponentSwizzle( VkComponentSwizzle const & value );
}
