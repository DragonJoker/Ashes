/*
This file belongs to Ashes.
See LICENSE file in root folder
*/
#pragma once

#include <ashes/common/FlagCombination.hpp>

namespace ashes::gl
{
	enum GlImageAspectFlag
		: GLenum
	{
		GL_DEPTH_BUFFER_BIT = 0x00000100,
		GL_STENCIL_BUFFER_BIT = 0x00000400,
		GL_COLOR_BUFFER_BIT = 0x00004000,
	};
	Ashes_ImplementFlag( GlImageAspectFlag )
	std::string getName( GlImageAspectFlags value );
	inline std::string toString( GlImageAspectFlags value ) { return getName( value ); }
	/**
	*\brief
	*	Convertit un VkImageAspectFlags en GlImageAspectFlags.
	*\param[in] flags
	*	Le VkImageAspectFlags.
	*\return
	*	Le GlImageAspectFlags.
	*/
	GlImageAspectFlags convertImageAspectFlags( VkImageAspectFlags flags );
}
