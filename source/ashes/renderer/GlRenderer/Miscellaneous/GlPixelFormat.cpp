#include "Core/GlContextLock.hpp"
#include "Miscellaneous/GlPixelFormat.hpp"

namespace ashes::gl
{
	PixelFormat::PixelFormat( ContextLock const & context
		, GlTextureType target
		, VkFormat vkformat
		, GlComponentMapping swizzle )
		: internal{ getInternalFormat( vkformat ) }
		, swizzle{ std::move( swizzle ) }
	{
		glLogCall( context, glGetInternalformativ
			, target
			, internal
			, GL_FORMAT_PROPERTY_TEXTURE_IMAGE_FORMAT
			, 1
			, reinterpret_cast< GLint * >( &format ) );
		glLogCall( context, glGetInternalformativ
			, target
			, internal
			, GL_FORMAT_PROPERTY_TEXTURE_IMAGE_TYPE
			, 1
			, reinterpret_cast< GLint * >( &type ) );

		if ( format == 0
			|| type == 0 )
		{
			format = getFormat( vkformat );
			type = getType( vkformat );
		}

		if ( type == GlType::GL_TYPE_UI8888_REV )
		{
			type = getType( vkformat );
		}
	}

	PixelFormat::PixelFormat()
	{
	}

	PixelFormat::PixelFormat( ContextLock const & context
		, GlTextureType target
		, VkFormat format )
		: PixelFormat{ context, target, format, getSwizzle( format ) }
	{
	}

	PixelFormat::PixelFormat( ContextLock const & context
		, GlTextureType target
		, VkFormat format
		, VkComponentMapping const & components )
		: PixelFormat{ context, target, format, getSwizzle( format, components ) }
	{
	}

	void PixelFormat::applySwizzle( ContextLock const & context
		, GlTextureType target )const
	{
		if ( swizzle.r != GL_COMPONENT_SWIZZLE_IDENTITY
			&& swizzle.r != GL_COMPONENT_SWIZZLE_RED )
		{
			glLogCall( context
				, glTexParameteri
				, target
				, GL_SWIZZLE_R
				, swizzle.r );
		}

		if ( swizzle.g != GL_COMPONENT_SWIZZLE_IDENTITY
			&& swizzle.g != GL_COMPONENT_SWIZZLE_GREEN )
		{
			glLogCall( context
				, glTexParameteri
				, target
				, GL_SWIZZLE_G
				, swizzle.g );
		}

		if ( swizzle.b != GL_COMPONENT_SWIZZLE_IDENTITY
			&& swizzle.b != GL_COMPONENT_SWIZZLE_BLUE )
		{
			glLogCall( context
				, glTexParameteri
				, target
				, GL_SWIZZLE_B
				, swizzle.b );
		}

		if ( swizzle.a != GL_COMPONENT_SWIZZLE_IDENTITY
			&& swizzle.a != GL_COMPONENT_SWIZZLE_ALPHA )
		{
			glLogCall( context
				, glTexParameteri
				, target
				, GL_SWIZZLE_A
				, swizzle.a );
		}
	}
}
