/*
This file belongs to Ashes.
See LICENSE file in root folder
*/
#pragma once

#include <Ashes/Enum/ImageLayout.hpp>

namespace gl_renderer
{
	enum GlImageLayout
		: GLenum
	{
		GL_LAYOUT_UNDEFINED_EXT = 0x0000,
		GL_LAYOUT_GENERAL_EXT = 0x958D,
		GL_LAYOUT_COLOR_ATTACHMENT_EXT = 0x958E,
		GL_LAYOUT_DEPTH_STENCIL_ATTACHMENT_EXT = 0x958F,
		GL_LAYOUT_DEPTH_STENCIL_READ_ONLY_EXT = 0x9590,
		GL_LAYOUT_SHADER_READ_ONLY_EXT = 0x9591,
		GL_LAYOUT_TRANSFER_SRC_EXT = 0x9592,
		GL_LAYOUT_TRANSFER_DST_EXT = 0x9593,
	};
	std::string getName( GlImageLayout value );
	/**
	*\brief
	*	Convertit un ashes::ImageLayout en VkImageLayout.
	*\param[in] layout
	*	Le ashes::ImageLayout.
	*\return
	*	Le VkImageLayout.
	*/
	GlImageLayout convert( ashes::ImageLayout const & layout );
	/**
	*\brief
	*	Convertit un VkImageLayout en ashes::ImageLayout.
	*\param[in] layout
	*	Le VkImageLayout.
	*\return
	*	Le ashes::ImageLayout.
	*/
	ashes::ImageLayout convertImageLayout( GlImageLayout layout );
}
