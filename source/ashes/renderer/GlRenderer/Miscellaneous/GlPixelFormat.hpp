/*
This file belongs to Ashes.
See LICENSE file in root folder
*/
#pragma once

#include "renderer/GlRenderer/GlRendererPrerequisites.hpp"

#include <array>

namespace ashes::gl
{
	class PixelFormat
	{
	private:
		PixelFormat( ContextLock const & context
			, GlTextureType target
			, VkFormat format
			, GlComponentMapping swizzle );

	public:
		PixelFormat();
		PixelFormat( ContextLock const & context
			, GlTextureType target
			, VkFormat format );
		PixelFormat( ContextLock const & context
			, GlTextureType target
			, VkFormat format
			, VkComponentMapping const & components );

		void applySwizzle( ContextLock const & lock
			, GlTextureType target )const;

		GlInternal internal{};
		GlFormat drawFormat{};
		GlType drawType{};
		GlFormat getFormat{};
		GlType getType{};
		bool readSupport{ false };
		GlFormat readFormat{};
		GlType readType{};
		GlComponentMapping swizzle{};
	};
}
